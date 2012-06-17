void Aerator_ON ()
{

 for (int i=1; i <= 3; i++){
 digitalWrite(2, HIGH);
 delay(100);
 digitalWrite(2, LOW);
 delay(900);
 }
 
 digitalWrite(8, HIGH); 
  
}

void Aerator_OFF ()
{

 digitalWrite(2, HIGH);
 delay(2000);
 digitalWrite(2, LOW);

 digitalWrite(8, LOW); 
  
}

void PUMP1_ON ()
{

 for (int i=1; i <= 2; i++){
 digitalWrite(2, HIGH);
 delay(300);
 digitalWrite(2, LOW);
 delay(700);
 }

 digitalWrite(6, HIGH); 
  
}

void PUMP1_OFF ()
{
 
 digitalWrite(2, HIGH);
 delay(2000);
 digitalWrite(2, LOW);

 digitalWrite(6, LOW); 
  
}

void PUMP2_ON ()
{

 for (int i=1; i <= 2; i++){
 digitalWrite(2, HIGH);
 delay(300);
 digitalWrite(2, LOW);
 delay(700);
 }

 digitalWrite(7, HIGH); 
  
}

void PUMP2_OFF ()
{
 
 digitalWrite(2, HIGH);
 delay(2000);
 digitalWrite(2, LOW);

 digitalWrite(7, LOW); 
  
}

void water_level_detection(void)
{
 
  if(digitalRead(3) == HIGH)
    digitalWrite(4,LOW);

  if(digitalRead(5) == LOW)
    digitalWrite(4,HIGH);

}
