//RAMAZAN EREN KARAKUŞ
//KAAN YİĞİT KAİM
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "Sizin Firebase Host İsminiz.."  
#define FIREBASE_AUTH "Firebase secret key.."  

const char *ssid = "Bağlanılacak olan ağın ismi..";       
const char *password = "Şifresi..";        
const char* host ="api.openweathermap.org"; 

//Veritabanı isim ve secret key güvenlik açısından paylaşılmamıştır.

String alinanveri1="";  
String alinanveri2="";  
String alinanveri3="";  
String alinanveri4="";   

//*************************************************************************************************

void setup() 
{
  Serial.begin(9600);   
  delay(1000);
  Serial.println("Openweather'a bağlanıyor..."); 
  
  WiFi.mode(WIFI_STA);   
  WiFi.begin(ssid,password); 

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
  Serial.println("İnternet bağlantısı sağlandı.");
  Serial.print("Cihazınızın güncel IP adresi: ");
  Serial.println(WiFi.localIP());  // kablosuz ağdan alınan IP adresi
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
}

//***************************************************************************************************

void loop() 
{
  WiFiClient istemci;  
  if (!istemci.connect(host, 80)) 
  {
    Serial.println("Bağlanti hatasi!!!");
    delay(3000);
    return;
  }
  Serial.println("Değerler güncelleniyor...");
  Serial.println("");
  
  String sehir = Firebase.getString("sehir");  
  
  //sehir.replace('"',' '); 
  sehir.remove(0,1);   
  sehir.remove(sehir.length()-1,1);     
  
  String uzanti="************"+sehir+",***************";
  //Verilerin çekildiği adres, mail atmanız durumunda özel olarak paylaşılacaktır. 
 
  istemci.print(String("GET ")+ uzanti + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
            
  unsigned long zamanasimi = millis();    
  while (istemci.available() == 0) 
  {                                                                      
    if (millis() - zamanasimi > 5000) 
    {                                    
      Serial.println(">>> Zaman asimi gerceklesti !");                    
      istemci.stop();
      delay(3000);
      return;
    }
  }

  while (istemci.available()) 
  {
    String satir = istemci.readStringUntil('\r');  
    

    if(satir.indexOf("list")!=-1) 
    {      
      Serial.print("Sehir: ");
      Serial.println(sehir);
      
      int baslangic = satir.indexOf("temp");  
      alinanveri1 ="";      
        for(int i=baslangic+6;i<baslangic+10;i++)
         alinanveri1+=satir[i];
         Serial.print("Hava Sıcaklığı: ");
         Serial.println(alinanveri1+"°C");
         Serial.println("----*----*----*----");
         
        
      int baslangic1 = satir.indexOf("feels_like"); 
      alinanveri2="";     
        for(int i=baslangic1+12;i<baslangic1+16;i++)
         alinanveri2+=satir[i];
         Serial.print("Hissedilen Sıcaklık: ");
         Serial.println(alinanveri2+"°C");
         Serial.println("----*----*----*----");
         
        
      int baslangic5 = satir.indexOf("humidity"); 
      alinanveri3="";   
        for(int i=baslangic5+10;i<baslangic5+12;i++)
         alinanveri3+=satir[i];
         Serial.print("Nem Oranı: ");
         Serial.println("%"+alinanveri3);
         Serial.println("----*----*----*----");
         

      int baslangic6 = satir.indexOf("speed");  
      alinanveri4="";        
        for(int i=baslangic6+7;i<baslangic6+11;i++)
         alinanveri4+=satir[i];
         Serial.print("Rüzgar Hızı: ");
         Serial.println(alinanveri4+" m/h");
         Serial.println("----*----*----*----");
      
      Firebase.setString("Hava Sıcaklığı", alinanveri1); 
      Firebase.setString("Hissedilen Sıcaklık", alinanveri2);
      Firebase.setString("Nem Oranı", alinanveri3);
      Firebase.setString("Yel Şiddeti", alinanveri4);
    }
  } 
  delay(1000);
}
