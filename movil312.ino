

#include <Arduino.h>
#include <WiFi.h> //libreria para conectar al wifi

#include <FirebaseClient.h>

#define WIFI_SSID "ARIEL" // nombre del router
#define WIFI_PASSWORD "12345" // contraseña del router


#define API_KEY "AIzaSyDkPlJcInxOtRHCZVUcdt8uLfndwxS-4_8"
#define USER_EMAIL a1122250011@utch.edu.mx"
#define USER_PASSWORD "12345678"
#define DATABASE_URL "https://echosafe-196b9-default-rtdb.firebaseio.com/"

//**CAMBIAR CAMPOS ANTERIORES SI LO QUIERE PROBAR**

void asyncCB(AsyncResult &aResult);

void printResult(AsyncResult &aResult);

DefaultNetwork network;

UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD);

FirebaseApp app;


#include <WiFiClientSecure.h>
WiFiClientSecure ssl_client;

using AsyncClient = AsyncClientClass;

AsyncClient aClient(ssl_client, getNetwork(network));

RealtimeDatabase Database;

bool taskComplete = false;

void setup()
{

    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //meter credenciales del wifi para conectarse

    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) //esperar a que se conecte
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP()); //Se puede mostrar la IP de donde te conectaste
    Serial.println();

    Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

    Serial.println("Initializing app...");

    initializeApp(aClient, app, getAuth(user_auth), asyncCB, "authTask");

    app.getApp<RealtimeDatabase>(Database);

    Database.url(DATABASE_URL);
}

void loop()
{


    app.loop();

    Database.loop();

    if (app.ready() && !taskComplete)
    {
        taskComplete = true;

        Database.set<String>(aClient, "/test/string", "CONECTADO CON WIFI :D", asyncCB, "pushStringTask");
        delay(100);

        Database.get(aClient, "/test/string", asyncCB, false, "getTask2");
    }
}

void asyncCB(AsyncResult &aResult)
{
    // WARNING!
    // Do not put your codes inside the callback and printResult.

    printResult(aResult);
}

void printResult(AsyncResult &aResult)
{
    if (aResult.isEvent())
    {
        Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.appEvent().message().c_str(), aResult.appEvent().code());
    }

    if (aResult.isDebug())
    {
        Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());
    }

    if (aResult.isError())
    {
        Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
    }

    if (aResult.available())
    {
        Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
    }
}