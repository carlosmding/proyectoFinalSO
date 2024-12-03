**Proyecto Sistemas Operativos - Arquitecturas en ESP32 con FreeRTOS**

Este proyecto tiene como objetivo comparar tres arquitecturas de manejo de interrupciones y control en un sistema basado en ESP32 utilizando FreeRTOS. Las arquitecturas se evalúan según la latencia de respuesta, uso de recursos, flexibilidad, y estabilidad. El proyecto está diseñado para simular un sistema de control que maneja interrupciones a través de diferentes métodos: prioridades estáticas, comunicación por colas y sincronización mediante semáforos.

**Estructura del Proyecto**

Este proyecto está compuesto por varias arquitecturas que simulan tareas de control e interrupción utilizando FreeRTOS. Las arquitecturas están implementadas en tareas separadas que simulan la interrupción y el control:

Arquitectura 1: Uso de prioridades estáticas para el manejo de interrupciones.

Arquitectura 2: Comunicación a través de colas para la gestión de tareas concurrentes.

Arquitectura 3: Sincronización mediante semáforos y temporizadores.

**Arquitecturas Implementadas**

Arquitectura 1: Prioridades Estáticas
En esta arquitectura, las tareas de interrupción y control tienen prioridades fijas. Se mide la latencia de respuesta al manejar las interrupciones y se calculan otras métricas como el uso de CPU y la memoria.


Arquitectura 2: Comunicación con Colas
Esta arquitectura utiliza colas para la comunicación entre tareas, lo que mejora la sincronización y flexibilidad al manejar múltiples interrupciones.


Arquitectura 3: Sincronización con Semáforos
En esta arquitectura, las tareas utilizan semáforos para la sincronización. Esto permite gestionar mejor las tareas de control e interrupción y medir la latencia de respuesta.

**Requisitos**

ESP32: El proyecto está diseñado para funcionar en un dispositivo ESP32.
FreeRTOS: Se utiliza para la gestión de tareas y sincronización en el sistema.
IDF (Espressif IoT Development Framework): Herramientas de desarrollo necesarias para compilar y cargar el proyecto en el ESP32.
Python: Requerido para interactuar con la herramienta esptool.

**Pasos para Configuración y Ejecución**

1. Clonar el Repositorio
Clona este repositorio en tu máquina local:

https://https://github.com/carlosmding/proyectoFinalSO

2. Configuración del Proyecto
   
Asegúrate de tener instaladas las dependencias necesarias, como el ESP-IDF. Si no tienes ESP-IDF instalado, sigue la guía oficial de instalación: Guía de instalación de ESP-IDF.

3. Compilar el Proyecto
   
Para compilar el proyecto, utiliza el siguiente comando:

idf.py build

4. Flashear el Proyecto al ESP32
   
Conecta tu dispositivo ESP32 y flashea el firmware con el siguiente comando:

idf.py -p COM5 flash

Asegúrate de reemplazar COM5 con el puerto correcto de tu dispositivo ESP32.


5. Monitorizar la Ejecución
   
Una vez flasheado el dispositivo, puedes monitorizar la salida en el puerto serial con:

idf.py -p COM5 monitor

Esto abrirá un monitor serial que te permitirá ver la salida del proyecto y las métricas de las tres arquitecturas.

**Resultados Esperados**

El proyecto imprime la latencia de respuesta para cada arquitectura, junto con otras métricas de desempeño como:

Promedio de latencia para cada arquitectura.

Uso de pila en cada tarea.

Tiempo total de ejecución para cada arquitectura.

Número de interrupciones procesadas.

Uso de CPU para cada arquitectura.



**Métricas**

Al final de la ejecución, se imprimen las siguientes métricas:


Promedio de latencias:

Arquitectura 1 - Promedio: XX ms

Arquitectura 2 - Promedio: YY ms

Arquitectura 3 - Promedio: ZZ ms


**Métricas adicionales:**

Arquitectura 1 - Uso de pila: XXX bytes

Arquitectura 2 - Uso de pila: YYY bytes

Arquitectura 3 - Uso de pila: ZZZ bytes


Arquitectura 1 - Tiempo total de ejecución: XXX ms

Arquitectura 2 - Tiempo total de ejecución: YYY ms

Arquitectura 3 - Tiempo total de ejecución: ZZZ ms


Arquitectura 1 - Número de interrupciones procesadas: X

Arquitectura 2 - Número de interrupciones procesadas: Y

Arquitectura 3 - Número de interrupciones procesadas: Z


Arquitectura 1 - Uso de CPU: XX%

Arquitectura 2 - Uso de CPU: YY%

Arquitectura 3 - Uso de CPU: ZZ%


Tamaño de heap antes de las tareas: XXX bytes

Tamaño de heap después de las tareas: YYY bytes


**Contribución**

Si deseas contribuir a este proyecto, por favor crea un pull request con tus cambios. Asegúrate de probar los cambios y documentar adecuadamente cualquier nueva característica o ajuste.
