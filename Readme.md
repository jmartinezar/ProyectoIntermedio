# Entropía en crema que se difunde en una taza de café

El presente trabajo hace parte de la entrega del proyecto intermedio correspondiente a la materia 'Introducción a la computación científica de alto rendimiento' dictada por el profesor William Fernando Oquendo Patiño, en el cual se aborda la simulación de un sistema físico correspondiente a una taza de café con crema sobre su superficie la cual, a medida que pasa el tiempo, se difunde sobre la misma, el propósito principal es analizar el comportamiento de la entropía presente en la crema a lo largo del tiempo, problema presentado en la sección 7.7 del libro 'Computational physics', de Nicholas J. Giordann y  Hisan Nikanishi. Para ello, se implementa el modelo computacional en c++ haciendo uso de herramientas vistas en el curso que permiten optimizar el código, una mejor detección de los problemas presentados a lo largo del desarrollo del mismo, conocer su funcionamiento, la forma en que interactúa con el hardward, etc.

# Modelación del sistema

El sistema fue modelado tomando en consideración dos partes del mismo: la taza de café y la crema. La taza de café, que es el espacio sobre el que se mueven las partículas, es modelado mediante una malla en cuyas divisiones puede haber una o más partículas. Las partículas son modeladas mediante su posición en la taza, por lo que el sistema es descrito con una cantidad de valores enteros correspondiente a la cantidad de partículas de crema. Si la partícula se encuentra en un borde y el paso a realizar la saca del recipiente, ésta ingresa por la parte contraria. Los índices son tomados tal que la posición 0 es tomada por la casilla superior izquierda mientras que la última es tomada por la casilla inferior derecha.

# Archivos de código

- `include.h`: Incluye los encabezados de las funciones a implementar.
- `include.cpp`: Incluye la implementación de las funciones.
- `main.cpp`: Incluye la ejecución del código con la solución de los puntos 1, 2 y 4.
- `input.txt`: Incluye los valores del sistema para los puntos 1 y 2.
- `input4.txt`: Incluye los valores del sistema para el punto 4.
- `*.gp`: Incluyen el código de gnuplot que genera la imagen correspondiente al punto.
- `Makefile`: Automatiza la ejecución del código y los reportes de profiling.

# Funciones implementadas

## `print_results`:
Almacena el valor de dos variables de tipo arbitrario, separados por un tab, y un salto de línea en un archivo txt.
Recibe:

- Variable tipo ofstream que indica el archivo txt en el que se guardan los valores.

- Dos variables de tipo arbitrario que son posteriormente escritas en el archivo txt.

Retorna: Nada.
	
## `cuatro_cuadros_centrados`:
Genera las condiciones iniciales para una crema ubicada en las cuatro posiciones centrales de la taza. Las moléculas son puestas en alguna de dichas cuatro casillas de forma aleatoria.
Recibe:
- La cantidad de moléculas de crema en el café. Tiene un valor entero y se etiqueta como `Nmol`.
- La longitud del lado de la taza de café (se modela una taza cuadrada). Tiene valor entero y se etiqueta como `size`.
- El vector que almacena la información respecto a posición de cada molécula de crema. Es pasado por referencia pues inicialmente el vector tiene todos los valores en cero y la función debe cambiar dichos valores de acuerdo a la condición inicial. Se etiqueta como `vector`.
- La semilla para la generación de números aleatorios. Es un entero y se etiqueta como `seed`.

Retorna: Nada.

## `move_particle`:
Habiendo sido tomada una partícula al azar y tomado uno de cinco movimientos posibles al azar (moverse a la derecha, a la izquierda, arriba, abajo y no moverse), se altera el vector que almacena la posición de las partículas para realizar el movimiento.
Recibe:
- Un valor aleatorio que determina la molécula a moverse. Es entero y se etiqueta como 'mol'.
- Un valor que determina el movimiento a realizar. Es entero y se etiqueta como 'paso'.
- La longitud del lado de la taza de café (se modela una taza cuadrada). Tiene valor entero y se etiqueta como 'size'.
- El vector que almacena la información respecto a posición de cada molécula de crema. Es pasado por referencia pues uno de sus valores puede ser alterado.

Retorna: Nada.

## `step`:
Simula un paso de tiempo.
Recibe:
- Un generador de números enteros aleatorios.
- Una distribución uniforme de enteros entre 0 y Nmol-1 (lo que representa las Nmol moléculas posibles pues los vectores en c++ son indexados iniciando de 0).
- Una distribución uniforme de enteros entre 0 y 4 (representado los cuatro estados posibles de movimiento).
- La longitud del lado de la taza de café (se modela una taza cuadrada). Tiene valor entero y se etiqueta como 'size'.
- El vector con el estado del sistema. Va pasado por referencia y es etiquetado como 'vector'.

Retorna: Nada.

## `find_t_eq`:
Halla el valor de t para el cual el sistema alcanza el equilibrio. Debido a la aleatoriedad en el comportamiento del sistema, se toma considera que el sistema alcanza el equilibrio cuando su valor es el 70% o más de su valor calculado teóricamente.
Recibe:
- La cantidad de moléculas de crema en el café. Tiene un valor entero y se etiqueta como 'Nmol'.
- La longitud del lado de la taza de café (se modela una taza cuadrada). Tiene valor entero y se etiqueta como 'size'.
- El vector que almacena la información respecto a posición de cada molécula de crema. Es pasado por referencia pues inicialmente el vector tiene todos los valores en cero y la función debe cambiar dichos valores de acuerdo a la condición inicial. Se etiqueta como 'vector'.
- La semilla para la generación de números aleatorios. Es un entero y se etiqueta como 'seed'.
- El número de pasos en el tiempo. Es entero y se etiqueta como 'Nsteps'.

Retorna: El tiempo de equilibrio. Es un valor entero.

## `evolution`:
Evoluciona el sistema a lo largo del tiempo y guarda en un archivo txt (usando print_results) los valores del paso de tiempo y la entropía, para el punto 1, y los valores del paso del tiempo y el radio de difusión, para el punto 3.
Recibe:
- La cantidad de moléculas de crema en el café. Tiene un valor entero y se etiqueta como 'Nmol'.
- La longitud del lado de la taza de café (se modela una taza cuadrada). Tiene valor entero y se etiqueta como 'size'.
- El vector que almacena la información respecto a posición de cada molécula de crema. Es pasado por referencia pues inicialmente el vector tiene todos los valores en cero y la función debe cambiar dichos valores de acuerdo a la condición inicial. Se etiqueta como 'vector'.
- La semilla para la generación de números aleatorios. Es un entero y se etiqueta como 'seed'.
- El número de pasos en el tiempo. Es entero y se etiqueta como 'Nsteps'.
- Variable tipo ofstream que indica el archivo txt en el que se guardan los valores.
- Un entero que indique el punto a solucionar correspondiente (1 o 3).

Retorna: Nada.

## `entropia`:
Calcula la entropía del sistema.
Recibe:
- La cantidad de moléculas de crema en el café. Tiene un valor entero y se etiqueta como 'Nmol'.
- El vector que almacena la información respecto a posición de cada molécula de crema.

Retorna: El valor de la entropía del sistema, tipo double.

## `radius`:
Calcula el radio de difusión del sistema (ver enunciado punto 3).
Recibe:
- La cantidad de moléculas de crema en el café. Tiene un valor entero y se etiqueta como 'Nmol'.
- El vector que almacena la información respecto a posición de cada molécula de crema.
- La longitud del lado de la taza de café (se modela una taza cuadrada). Tiene valor entero y se etiqueta como 'size'.

Retorna: El radio de difusión de la crema.

## `move_with_hole`:
Habiendo sido tomada una partícula al azar y tomado uno de cinco movimientos posibles al azar (moverse a la derecha, a la izquierda, arriba, abajo y no moverse), se altera el vector que almacena la posición de las partículas para realizar el movimiento, con la salvedad de que ahora en la parte inferior derecha del recipiente hay una cabidad por la que pueden salir las moléculas.
Recibe:
- Un valor aleatorio que determina la molécula a moverse. Es entero y se etiqueta como 'mol'.
- Un valor que determina el movimiento a realizar. Es entero y se etiqueta como 'paso'.
- La longitud del lado de la taza de café (se modela una taza cuadrada). Tiene valor entero y se etiqueta como 'size'.
- El vector que almacena la información respecto a posición de cada molécula de crema. Es pasado por referencia pues uno de sus valores puede ser alterado.
- Valor que representa la proporción de la cabidad, para valores mayores la cabidad será menor y viceversa. Es entero y se etiqueta como 'ratio'.
- La cantidad de moléculas en el recipiente. Es entero y se etiqueta como 'numero'.

Retorna: Nada.

## `step_with_hole`:
Realiza un paso temporal para el recipiente con un hueco.
Recibe:
- Un generador de números enteros aleatorios.
- Una distribución uniforme de enteros entre 0 y Nmol-1 (lo que representa las Nmol moléculas posibles pues los vectores en c++ son indexados iniciando de 0).
- Una distribución uniforme de enteros entre 0 y 4 (representado los cuatro estados posibles de movimiento).
- La longitud del lado de la taza de café (se modela una taza cuadrada). Tiene valor entero y se etiqueta como 'size'.
- El vector con el estado del sistema. Va pasado por referencia y es etiquetado como 'vector'.
- Valor que representa la proporción de la cabidad, para valores mayores la cabidad será menor y viceversa. Es entero y se etiqueta como 'ratio'.
- La cantidad de moléculas en el recipiente. Es entero y se etiqueta como 'numero'.

Retorna: Nada.

## `with_hole`:
Realiza la evolución temporal para el sistema con un hueco.
Recibe:
- La longitud del lado de la taza de café (se modela una taza cuadrada). Tiene valor entero y se etiqueta como 'size'.
- El vector que almacena la información respecto a posición de cada molécula de crema. Es pasado por referencia pues inicialmente el vector tiene todos los valores en cero y la función debe cambiar dichos valores de acuerdo a la condición inicial. Se etiqueta como 'vector'.
- La semilla para la generación de números aleatorios. Es un entero y se etiqueta como 'seed'.
- El número de pasos en el tiempo. Es entero y se etiqueta como 'Nsteps'.
- Valor que representa la proporción de la cabidad, para valores mayores la cabidad será menor y viceversa. Es entero y se etiqueta como 'ratio'.
- Variable tipo ofstream que indica el archivo txt en el que se guardan los valores.