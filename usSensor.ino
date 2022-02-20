const int echoPin = 2;
const int trigPin = 3;

long duration;
int distance;

boolean prevState = false;
int ticksSinceLastChange = 0;

const int debounceStrength = 3; //How many stable cycles to wait until the state is changed
const boolean debug = false; //Prints out the above two variables

void setup() 
{
  Serial.begin(9600); //Starts the serial connection
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	pinMode(LED_BUILTIN, OUTPUT); //Sets up the pins used and the built in led
}

void loop()
{  
	digitalWrite(trigPin, HIGH); //Starts the pulse on the trig pin
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);

	duration = pulseIn(echoPin, HIGH); //Measures the returned pulse time
	distance = duration * 0.034 / 2; //Converts the time to cm

  boolean debouncedState = report();
  if(debouncedState)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("1");
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("0");
  }
	delay(10);
}

boolean report() //Accounts for bounce and returns the processed state
{
  boolean cState = true;
  if (distance < 50) cState = false;

  if (debug)
  {
    Serial.print("Last stable state: ");
    Serial.println(prevState);
    Serial.print("Time of current stable state: ");
    Serial.println(ticksSinceLastChange);
  }
  
  if (cState != prevState)
  {
    if (ticksSinceLastChange > debounceStrength)
    {
      prevState = cState;
      ticksSinceLastChange = 0;
      return cState;
    }
    else
    {
      ticksSinceLastChange++;
      return prevState;
    }
  }
  else
  {
    ticksSinceLastChange = 0;
    return cState;
  }
}
