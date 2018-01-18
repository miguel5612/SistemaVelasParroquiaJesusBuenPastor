    const int velas[] = {22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53};
    const int sensor[] = {2,3,4};
    const int velasEspeciales[] = {9,18,27,random(1,32)};
    const int releEspecial = 5;
    const int releMP3 = 6;
    int tiempoEspecial = 0;
    int tiempoEspecialMax = random(60,300); // 2 minutos
    const boolean estadoApagado = true;
    int tiempoEncendido = 60; // un minuto
    const int maxVelas = 32;
    const int maxSensores = 3;
    const int mute = 7;
    boolean estado = false, algunaEncendida = false, especialEncendido = false;
    int numeroVela=0;
    int tiempoVelas[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int contador = 0;
    double  tiempoAnterior = 0;
    
    void setup()
     {

    Serial.begin(9600);

    Serial.print("Maximo de velas: ");
    Serial.println(maxVelas);
    Serial.print("Maximo de sensores: ");
    Serial.println(maxSensores);
    //declaro las entradas y salidas
    for(int i = 0; i<maxVelas ; i++)
      {
        pinMode(velas[i],OUTPUT);
        digitalWrite(velas[i],estadoApagado);
        Serial.print("Configurando salida: ");
        Serial.println(velas[i]);
      }

    for(int i = 0; i<maxSensores ; i++)
      {
        pinMode(sensor[i],INPUT);
        Serial.print("Configurando la entrada: ");
        Serial.println(sensor[i]);
      }
      pinMode(releMP3,OUTPUT);
      pinMode(releEspecial,OUTPUT);

      //escribo los estados por defecto

      digitalWrite(releMP3, LOW);
      digitalWrite(releEspecial, LOW);

    }
    void loop()
    {
      leerSensores();
      contarTiempo(algunaEncendida);
      apagarEspecial();
      /*if(mute){
      
          digitalWrite(releEspecial, HIGH);
          digitalWrite(releMP3, true);
      }*/
    }

    void leerSensores(){
      for(int k=0; k<maxSensores ; k++){
          estado = digitalRead(sensor[k]);
          if(estado){
            Serial.print("Se ha leido un pulso del sensor que esta en el pin: ");
            Serial.println(sensor[k]);
            Serial.println("**************************************************");
            numeroVela = encenderVela(estado);
        algunaEncendida = true; //encendi una vela :)
        comprobarEspecial(numeroVela);
        Serial.print("Encendida la vela numero: ");
        Serial.println(numeroVela);
        delay(200);
          }
        } 
    }

    int encenderVela(boolean estado){
   
        if(contador<maxVelas){
          tiempoVelas[contador] = tiempoVelas[contador] + tiempoEncendido;
          Serial.print("Encendida la salida numero: ");
          Serial.print(velas[contador]);
          Serial.print ("  con tiempo: ");
          Serial.println(tiempoVelas[contador]);
          digitalWrite(velas[contador],!estadoApagado);
          algunaEncendida = true;
          contador ++;
          return contador; // numeroVela
        }else{
          contador = 0;
          tiempoVelas[contador] = tiempoVelas[contador] + tiempoEncendido;
          Serial.print("Encendida la salida numero: ");
          Serial.print(velas[contador]);
          Serial.print ("  con tiempo: ");
          Serial.println(tiempoVelas[contador]);
          digitalWrite(velas[contador],!estadoApagado);
          algunaEncendida = true;
          return contador; // numeroVela
        
          tiempoEncendido = tiempoEncendido + tiempoEncendido/2;
        }
      
    }

void contarTiempo(boolean status){
    if(status || tiempoEspecial>0){
        //empiezo a contar y a disminuir segundo a segundo
        if(tiempoAnterior == 0){
          tiempoAnterior = millis();
        }else if(millis()-tiempoAnterior >= 1000){
          tiempoAnterior = 0;
          boolean test = false;
            //empiezo a descontar segundo a segundo para todas las velas encendidas
            if(status){
              for(int j = 0;j<=maxVelas;j++){
                  if(tiempoVelas[j] > 0){
                    tiempoVelas[j] = tiempoVelas[j] - 1;
                    test = true;
                  }else{
                    tiempoVelas[j] = 0;
                    digitalWrite(velas[j],estadoApagado);
                  }

              }
            }
            if(tiempoEspecial>0){
                tiempoEspecial = tiempoEspecial - 1;
                especialEncendido = true;
                  Serial.print("EL tiempo especial es: ");
                  Serial.println(tiempoEspecial);
                  test = true;
            }
            if(!test){
              algunaEncendida = false;
              Serial.println("Todas Apagadas");
              tiempoEncendido = 60;
            } 
        }
    }
    }
    void apagarEspecial(){
      if((especialEncendido && tiempoEspecial <= 0) ||  (tiempoEspecial <= 0 && (digitalRead(releMP3) || digitalRead(releEspecial)))){
              especialEncendido = false;
              digitalWrite(releEspecial, LOW);
              digitalWrite(releMP3, LOW);
              tiempoEspecial = 0;
              Serial.println("especial se ha apagado");
            }
    }
    void comprobarEspecial(int numero){
      for(int l=0; l<5 ;l++){
        if(numero == velasEspeciales[l]){
          Serial.println("Especial!!!!");
          especialEncendido = true;
          digitalWrite(releEspecial, HIGH);
          digitalWrite(releMP3, true);
          if(tiempoEspecial<tiempoEspecialMax){ tiempoEspecialMax = random(60,500);}
          tiempoEspecial = tiempoEspecial + tiempoEspecialMax;
          Serial.print("Tiempo Especial= ");
          Serial.println(tiempoEspecial);
        }
      }
    }