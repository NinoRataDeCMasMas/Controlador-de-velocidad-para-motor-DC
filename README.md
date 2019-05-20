# Controlador de velocidad para motor DC

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

