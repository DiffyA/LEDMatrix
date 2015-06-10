// PixelStripsUPDATED.ino

#include <Adafruit_NeoPixelUPDATED.h> // NeoPixel class including animations implemented as methods.
#include <avr/power.h>

#define PIN 4 // Data wire.

Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, PIN); // We declare this Adafruit_NeoPixel object composed of 20 LEDs.

char rx_byte; // Byte to be received.
String rx_str = ""; // String to be received.

boolean colorWipeRandom = false;
String functions[] = {"stop", "colorWipeRandom", "turnOnRandomColor", "pulseEffect", "pulseEffectMultiple", 
                      "rainbow", "rainbowCycle", "theaterChaseRainbow"}; // Array containing the name of the allowed functions.
int parameters[] =  {0, 2, 2, 2, 3,
                     1, 1, 1 }; // Amount of parameters needed for each function in the functions[] array.
int rx_parameters[3]; // Array storing the values received as parameters. Maximum value in the parameters[] array is the size of this received parameters array.
boolean settings[sizeof(functions)]; // Boolean array containing the true or false values pertaining to each of the separate functions.

void setup() {
  for (int ii = 0; ii < sizeof(settings); ii++){ // Sets our boolean array to start on false for every value.
    settings[ii] = false;
  }
  Serial.begin(9600); // Initialize serial communications.
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  checker();
  if (rx_byte == '\n'){ // Only check once we have introduced a command, which is when a newline is found.
    if (settings[0]){ // Stop was reached.
      for (int ii = 0; ii < strip.numPixels(); ii++){
        strip.setPixelColor(ii, 0,0,0);
      }
      strip.show();
    }
    else if (settings[1]){ // colorWipeRandom(uint16_t wait, byte brightness)
      strip.colorWipeRandom(rx_parameters[0], rx_parameters[1]);
    } 
    else if (settings[2]){ // turnOnRandomColor (uint16_t wait, byte brightness)
      strip.turnOnRandomColor(rx_parameters[0], rx_parameters[1]);
    }
    else if (settings[3]){ // pulseEffect(uint16_t hold, uint16_t wait)
      for (int ii = 0; ii < strip.numPixels(); ii++){ // Refreshes the strip, sets everything off. This is needed because if we come from colorWipeRandom for example, the pixels would all still be on while certain ones pulse.
        strip.setPixelColor(ii, 0,0,0);
      }
      strip.show();
      strip.pulseEffect(rx_parameters[0], rx_parameters[1]);
    }
    else if (settings[4]){ // pulseEffectMultiple(uint32_t amount, uint16_t hold, uint16_t wait)
      strip.pulseEffectMultiple(rx_parameters[0], rx_parameters[1], rx_parameters[2]);
    }
    else if (settings[5]){ // rainbow(uint8_t wait)
      strip.rainbow(rx_parameters[0]);
    }
    else if (settings[6]){ // rainbowCycle(uint8_t wait)
      strip.rainbowCycle(rx_parameters[0]);
    }
    else if (settings[7]){ // theaterChaseRainbow(uint8_t wait);
      strip.theaterChaseRainbow(rx_parameters[0]);
    }
  }
}

String* checker (){
	if (Serial.available() > 0) {    // is a character available?
      rx_byte = Serial.read();       // get the character
    if (rx_byte != '\n') {  // A character of the string was received.
      rx_str += rx_byte;
    }
    else { // End of string, start analyzing.
      rx_str.trim(); // Trims the received string of leading and trailing whitespaces.
      int startPos = rx_str.indexOf('('); // Gets the position of the first parenthesis.
      int endPos = rx_str.lastIndexOf(')'); // Gets the position of the last parenthesis.
      int parameterCounter; // Counts the amount of parameters in our string.
      String parameters_str = rx_str.substring(startPos+1, endPos); // Stores the parameter information inside of this string.
      String method_str = rx_str.substring(0, startPos); // Stores the method information inside of this string.

      parameters_str.trim(); // Trims the string of leading and trailing whitespaces;

      //Format check: checks if opening and closing parenthesis are present.
      if ((startPos == -1 || endPos == -1) || method_str.length() == 0){ // If these variables aren't capable of obtaining values, means a parenthesis was not found so the input is incorrect.
        Serial.println("Input is incorrect.");
        rx_str = "";
        parameters_str = "";
        method_str = ""; // Resets all the variables.
      }
      else{ // If input was accepted, continue.
        if (parameters_str.length() == 0){ // If there is nothing left inside the parameter string after trimming, there are 0 parameters.
          parameterCounter = 0;
        }
        else { // Else, there exists at least one, so our counter starts with that value.
          parameterCounter = 1;
        }
        for (int ii = 0; ii < parameters_str.length(); ii++){ // Counts the amount of parameters in the string, using commas as a delimiter.
          if (parameters_str.charAt(ii) == ','){
            parameterCounter++;
          }
        }

        String parameters_arr[parameterCounter+1]; // Create the array that will hold all of our parameters and the name of the method.
        parameters_arr[0] = method_str; // Sets the name of the method as the first position in our array.

        for (int counter = 1; counter < parameterCounter + 1; counter++){ // Do this whole thing until we fill up the array.
          String currentParameter = "";
          if (parameterCounter - counter == 0 || parameterCounter == 1){ // Do this only when there is one parameter left (we can no longer use commas as delimiters).
            parameters_arr[counter] = parameters_str; // Send the string (or what's left of it) to the array.
          }
          else{ // We still have commas.
            for (int ii = 0; ii < parameters_str.length(); ii++){ // Run through the string containing the parameters.
              if (parameters_str.charAt(ii) == ','){ // Gets the position of the first comma
                endPos = ii;
                break;
              }
            }
            currentParameter = parameters_str.substring(0,endPos); // Sets the current parameter from the beginning of the string to the first found comma.
            parameters_arr[counter] = currentParameter; // Gives that value to the corresponding array position.
            parameters_str.remove(0, endPos + 1); // Remove "endpos" amount of characters from the beginning of the string, effectively removing everything up to the first comma.
          }
        }

        // Uncomment this if you need to check the contents of the array up to here.
        // Serial.println("Array contents: ");

        // for (int ii = 0; ii < parameterCounter+1; ii++){
        //   Serial.println(parameters_arr[ii]);
        // }

        // Analysis of the string is done. This is when we start interacting with the global arrays to set a function to TRUE or FALSE.
        String toActivate = parameters_arr[0]; // Name of the function we want to activate stored in the array's first position.
        int position; // Position in the functions array where the function we want to select is stored.

        for (int ii = 0; ii < sizeof(functions); ii++){ // Searches for the position of the function in the functions array.
          if (toActivate.equalsIgnoreCase(functions[ii])){
            position = ii;
            break; // Gets out of this for as soon as the if condition is met.
          }
          else {
            position = -1; // If this happens, the specified function name was not found within the existing functions.
          }
        }
        

        if (position == -1){ // If the function name was not found, print this error message.
            Serial.println("ERROR: Function not found within the existing functions.");
          }
          else { // If the function name was found, move on.
        if (parameters[position] != parameterCounter){ // If the specified number of parameters isn't the same as the needed for that given function.
          Serial.println("ERROR: Incorrect number of parameters.");
          Serial.println("Function: " + method_str);
          Serial.print("Number of needed parameters: " );
          Serial.println(parameters[position]);
          Serial.print("Number of specified parameters: ");
          Serial.println(parameterCounter);
        }
        else{ // If the number of specified parameters and the amount of needed parameters are the same, move on, all checks are done.
          for (int ii = 0; ii < parameterCounter; ii++){ // Passing the necessary values from our local parameters[] array to our global rx_parameters[] array.
            rx_parameters[ii] = parameters_arr[ii+1].toInt();
          }
          for (int ii = 0; ii < parameterCounter; ii++){ // Passing the necessary values from our local parameters[] array to our global rx_parameters[] array.
            Serial.println(rx_parameters[ii]);
          }
          for (int ii = 0; ii < sizeof(settings); ii ++){ // Runs through the array holding the boolean values for each of the functions.
            if (ii == position){ // Once we get to the position set it to true.
              settings[ii] = true;
            }
            else { // Set all other positions to false.
              settings[ii] = false;
            }
          }
        }
      }
    }
    rx_str = "";
    parameters_str = "";
    method_str = "";
    } // end : else of the if (Serial.available() > 0);
  } // end: if (Serial.available() > 0)
}  // end: checker()
