# Controlador de velocidad para motor DC

![](https://github.com/NinoRataDeCMasMas/Controlador-de-velocidad-para-motor-DC/blob/master/schematics/prototype.jpg)

## Introducción
En el presente repositorio se centra el material para crear un prototipo de controlador de velocidad para motores DC implementando Arduino. Con el se pretende hacer experimentación de algoritmos de control automático y procesamiento de señales, específicamente en filtros.

## Implementación de Hardware

![](https://github.com/NinoRataDeCMasMas/Controlador-de-velocidad-para-motor-DC/blob/master/schematics/schematic.png)

El prototipo implementa la tecnología Arduino como el microcontrolador que leerá la velocidad seleccionada por el usuario a través de un potenciómetro o resistencia variable, e inyectar energía al motor de corriente directa para generar las revoluciones deseadas. El esquema eléctrico se muestra a continuación.

![](https://github.com/NinoRataDeCMasMas/Controlador-de-velocidad-para-motor-DC/blob/master/schematics/diagram.png)

Como sistema se emplea un motor de la marca [Pololu 37D](https://www.pololu.com/category/116/37d-mm-metal-gearmotors), el cual implementa de manera interna un [encoder de cuadratura](https://www.puntoflotante.net/FUNCIONAMIENTO-ENCODER-CUADRATURA-EFECTO-HALL.htm) que se vale del _efecto Hall_ para la medicion de las revoluciones del motor.

Como interfaz entre la etapa de potencia requerida para el funcionamiento del motor y la etapa de control se implementa un transistor [TIP122](https://www.onsemi.com/pub/Collateral/TIP120-D.PDF) como válvula electrónica, y el microcontrolador usa la tecnología [PWM](https://es.wikipedia.org/wiki/Modulaci%C3%B3n_por_ancho_de_pulsos) para controlar la potencia suministrada al motor.

## Implementación de Software

Una vez construida la implementación de Hardware se desarrollo una serie de programas para la funcionalidad del sistema. Se implemento la [Programacion Orientada a Objetos](https://es.wikipedia.org/wiki/Programaci%C3%B3n_orientada_a_objetos) por lo que partes del sistema fueron abstraídas en clases, como se muestra en el siguiente diagrama en UML:

![](https://github.com/NinoRataDeCMasMas/Controlador-de-velocidad-para-motor-DC/blob/master/schematics/motorSystemClass.png)

En esta parte del proyecto de anadio una pantalla LCD con conexion i2c para la visualizacion de los parametros, ya que el monitor serial del Arduino se ocuparia para otros propositos.

## Instalación del proyecto
Descarga los contenidos de este repositorio en tu computadora. Mueva los directorios
 * _ComplementaryFilter_
 * _Encoder_

al directorio de librerias de Arduino. El directorio llamado _sketch_ contiene el programa principal y la clase _MotorSYstem_. MOdifique dicha clase a sus necesidades y cargue en el arduino el archivo _sketch.ino_. 

## Implementando un lazo abierto

![](https://github.com/NinoRataDeCMasMas/Controlador-de-velocidad-para-motor-DC/blob/master/schematics/system.png)

El siguiente ejemplo implementa un control a lazo abierto del sistema, mostrado en el esquema anterior:

```C++

#include "MotorSystem.h"

const long baudage = 115200;

MotorSystem* motorSystem;

void setup( void )
{
  Serial.begin(baudage);
  motorSystem = new MotorSystem(9, A1, 3, 4);

  auto isr = []( void ) -> void { motorSystem->begin(); };
  attachInterrupt(digitalPinToInterrupt(motorSystem->interruptPin()), isr, CHANGE);
}

void loop( void )
{
  motorSystem->run();
}
```
Donde el metodo _run()_ ejecuta el siguiente codigo:

```C++

void MotorSystem::run( void )
{
	// we read pwm value from potentiometer. the value contains
	// an offset that serve as on/off switch.
	auto pot = map(analogRead(potPin), 0, 1023, 0, 255 + offset);

	if(pot < offset)
	{
		// A "pot" value less than offset indicates system in off mode
		analogWrite(motorPin, 0);

		// show data
		lcd->setCursor(0, 0);
		lcd->print(" motor  apagado ");
		lcd->setCursor(0, 1);
		lcd->print("                ");     
	}
	else
	{
		// A "pot" value greater than offset indicates system in on mode.
		// filtering pot value without offset    
		auto pwm = filter->compute(pot - (offset + .0));  
		analogWrite(motorPin, pwm);

		// show data
		lcd->setCursor(0, 0);
		lcd->print("motor  encendido");  
		lcd->setCursor(0, 1);
		lcd->print("U:" + (String)pwm);
		lcd->setCursor(9, 1);
		lcd->print("Y:" + (String)encoder->pulseXsec());    
	}
}
```

## Implementando un lazo abierto
Para implementar un sistema de lazo cerrado como el que se muestra en el siguiente esquema se propone conocer el comportamiento de la planta o sistema, en este caso el motor DC. 

![](https://github.com/NinoRataDeCMasMas/Controlador-de-velocidad-para-motor-DC/blob/master/schematics/systemRetro.png)

Para ello se dispone a utilizar un metodo llamado _generateCuerve_ el cual proporcionara al sistema todo el rango dinamico del PWM al motor y leera la respuesta en _cuentas por unidad de tiempo_ descrita en la clase _Encoder_. Utilizando el siguiente script en lenguaje R se pudo graficar y determinar la region lineal del sistema para asi acotar su entrada:

```R
# Take all data set measured by motor response
raw  <- read.csv("motorModel.csv")
plot(raw$RPM, raw$PULSES, col = "red", ylab = "encoder pulses", xlab = "pwm signal")

# select data in range 0...100, previous analysis
data = read.csv("motorLinearModel.csv")
# generate a linear equations 
model     <- lm(data$PULSES ~ data$RPM)
antiModel <- lm(data$RPM ~ data$PULSES)

# show results
plot(data$RPM, data$PULSES, col = "green", ylab = "encoder pulses", xlab = "pwm signal")
abline(model)

# data analysis
summary(model)
summary(antiModel)
anova(model)
```

![](https://github.com/NinoRataDeCMasMas/Controlador-de-velocidad-para-motor-DC/blob/master/schematics/motorResponse.png)

![](https://github.com/NinoRataDeCMasMas/Controlador-de-velocidad-para-motor-DC/blob/master/schematics/linearRegion.png)


Del analisis obtenido generamos ecuaciones lineales para la conversion entre PWM y pulsos por tiempo. 


