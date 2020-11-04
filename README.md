# ARCHIVE-12
THIS IS PART OF ARCHIVE SERIES A CLOUD STORAGE FOR OUR STARTUP FILES WHICH U CAN RESEARCH :-)

rough codes: 






..............................................................................
int m1 = 5 ;
int m2 = 6 ;

void setup () {

pinMode ( m1,OUTPUT ) ;
pinMode ( m2 , OUTPUT ) ; 

}

void loop () { 

digitalWrite ( m1 , HIGH );
digitalWrite ( m2 , HIGH );
delay (30000);
digitalWrite ( m1 , HIGH );
digitalWrite ( m2 , LOW );
delay (30000);
digitalWrite ( m1 , LOW );
digitalWrite ( m2 , HIGH );
delay ( 30000 ) ;
digitalWrite ( m1 , LOW );
digitalWrite ( m2 , LOW );
delay ( 30000 ) ;
}



