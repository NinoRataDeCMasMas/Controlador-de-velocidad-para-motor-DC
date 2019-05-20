# Controlador de velocidad para motor DC

## Introducción
En el presente repositorio se centra el material para crear un prototipo de controlador de velocidad para motores DC implementando Arduino. Con el se pretende hacer experimantación de algoritmos de control automático y procesamiento de señales, especificamente en filtros.

## Implementación de Hardware

![](https://github.com/NinoRataDeCMasMas/Controlador-de-velocidad-para-motor-DC/blob/master/schematics/schematic.png)

El prototipo implementa la tecnologia Arduino como el microcontrolador que leera la velocidad seleccionada por el usuario a traves de un potenciometro o resistencia variable, e inyecctara energia al motor de corriente directa para generar las revoluciones deseadas. El esquema electrico se muestra a continuacion.

![](https://github.com/NinoRataDeCMasMas/Controlador-de-velocidad-para-motor-DC/blob/master/schematics/diagram.png)

Como sistema se emplea un motor de la marca [Pololu 37D](https://www.pololu.com/category/116/37d-mm-metal-gearmotors), el cual implementa de manera interna un [encoder de cuadratura](https://www.puntoflotante.net/FUNCIONAMIENTO-ENCODER-CUADRATURA-EFECTO-HALL.htm) que se vale del _efecto Hall_ para la medicion de las revoluciones del motor.

