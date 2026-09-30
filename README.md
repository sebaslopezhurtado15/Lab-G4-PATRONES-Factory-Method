# PATRÓN SINGLETON — SIS457 - Grupo 4

Proyecto desarrollado en **Unreal Engine 4.27.2** utilizando **C++**.

Este trabajo implementa el patrón de diseño **Singleton** tomando como referencia el ejemplo **“Programming Elitists’ Bane… Singleton Pattern”** del libro *Unreal Engine 5 Game Programming Design Patterns in C++, Java, C#, and Blueprints*.

El objetivo es comprobar de forma práctica que, aunque se intente crear varias veces un mismo objeto, solamente quede **una instancia activa** dentro del juego.

---

## 1. Objetivo del proyecto

Implementar y analizar el patrón de diseño **Singleton** dentro del proyecto `AventuraUSFX022026L4`, utilizando una solución sencilla y compatible con **Unreal Engine 4.27.2**.

Para la práctica se utilizan principalmente:

- `AActor`.
- `SpawnActor()`.
- `TArray`.
- `UGameplayStatics::GetAllActorsOfClass()`.
- `Cast`.
- `Destroy()`.
- `UPROPERTY`.
- `BeginPlay()`.
- `GEngine->AddOnScreenDebugMessage()`.

---

## 2. Clases principales

Se crearon las clases:

- `ASingleton_Main`
- `AInventory`

### `ASingleton_Main`

Esta clase se utiliza para realizar la prueba del patrón.

En `BeginPlay()` intenta crear varias instancias de `AInventory` mediante:

```cpp
GetWorld()->SpawnActor<AInventory>(AInventory::StaticClass());
```

El ciclo utilizado es:

```cpp
for (int i = 0; i <= 4; i++)
```

Por lo tanto, se realizan **5 intentos de creación**.

Cada vez que un `Inventory` es creado correctamente, se guarda su referencia y se muestra un mensaje en pantalla.

---

## 3. Clase Inventory

La clase `AInventory` es la clase sobre la que se aplica la lógica del patrón Singleton.

Cada vez que se crea un nuevo `Inventory`, se buscan las instancias existentes mediante:

```cpp
TArray<AActor*> Instances;

UGameplayStatics::GetAllActorsOfClass(
    GetWorld(),
    AInventory::StaticClass(),
    Instances
);
```

Después se comprueba:

```cpp
if (Instances.Num() > 1)
```

Si existen más de una instancia, significa que ya había un `Inventory` creado anteriormente.

Entonces se guarda la referencia al primero:

```cpp
Instance = Cast<AInventory>(Instances[0]);
```

y el nuevo objeto se elimina mediante:

```cpp
Destroy();
```

De esta manera, aunque se intente crear varios `Inventory`, solamente permanece uno.

---

## 4. Funcionamiento

El funcionamiento general es:

```text
Inicio
  |
  v
Singleton_Main ejecuta BeginPlay()
  |
  v
Intenta crear varios Inventory
  |
  v
Cada Inventory busca cuántos existen
  |
  +----------------------------+
  |                            |
  v                            v
Solo existe 1             Existen más de 1
  |                            |
  v                            v
Se mantiene              Guarda el primero
                               |
                               v
                         Destroy() al nuevo
                               |
                               v
                    Solo queda una instancia
```

---

## 5. Comprobación del patrón

Durante la ejecución aparecen mensajes similares a:

```text
Inventory_0 has been created
Inventory_0 already exists
Inventory_0 already exists
Inventory_0 already exists
Inventory_0 already exists
```

Esto demuestra que:

1. El primer `Inventory` se crea correctamente.
2. Los siguientes intentos detectan que ya existe uno.
3. Los nuevos objetos son destruidos.
4. Al final solamente queda un `Inventory` activo.

También se puede comprobar en el **World Outliner**, donde permanece una sola instancia de `Inventory`.

---

## 6. Representación visual

Para facilitar la demostración se agregó un `UStaticMeshComponent` a `AInventory`.

Se utiliza el cubo básico de Unreal Engine únicamente como representación visual:

```cpp
static ConstructorHelpers::FObjectFinder<UStaticMesh> Cubo(
    TEXT("/Engine/BasicShapes/Cube.Cube")
);
```

La malla no modifica la lógica del patrón Singleton.

Su finalidad es permitir comprobar visualmente que, aunque existan varios intentos de creación, solamente queda **un objeto visible**.

---

## 7. Archivos principales

```text
Source/AventuraUSFX022026L4/
│
├── Singleton_Main.h
├── Singleton_Main.cpp
├── Inventory.h
└── Inventory.cpp
```

### `Singleton_Main.h`

Contiene la declaración de la clase `ASingleton_Main` y el puntero:

```cpp
AInventory* Inventory;
```

### `Singleton_Main.cpp`

Contiene la lógica que intenta crear varios objetos `AInventory` mediante `SpawnActor()`.

### `Inventory.h`

Contiene la declaración de `AInventory`, el puntero:

```cpp
AInventory* Instance;
```

y el componente utilizado para representar visualmente el objeto.

### `Inventory.cpp`

Contiene la lógica que:

- busca las instancias existentes;
- comprueba cuántas hay;
- guarda la primera instancia;
- destruye las instancias adicionales.

---

## 8. Conceptos utilizados

En esta práctica se aplican conceptos de Programación Orientada a Objetos y C++:

- **Clase:** `ASingleton_Main` y `AInventory`.
- **Objeto:** cada instancia creada de `AInventory`.
- **Herencia:** ambas clases heredan de `AActor`.
- **Punteros:** `Inventory` e `Instance`.
- **Métodos:** `BeginPlay()`, `Tick()` y `Destroy()`.
- **Patrón de diseño:** Singleton.
- **Arreglos de Unreal:** `TArray`.
- **Casting:** `Cast<AInventory>()`.

También se utilizan elementos propios de Unreal Engine 4.27.2:

- `AActor`.
- `SpawnActor()`.
- `GetWorld()`.
- `GetAllActorsOfClass()`.
- `CreateDefaultSubobject()`.
- `UStaticMeshComponent`.
- `GEngine->AddOnScreenDebugMessage()`.

---

## 9. ¿Por qué es Singleton?

La idea del patrón Singleton es permitir que exista una sola instancia de una clase.

En esta implementación:

- `Singleton_Main` intenta crear varios objetos.
- `Inventory` comprueba si ya existe otro objeto de su misma clase.
- Si encuentra más de uno, destruye el nuevo.
- El resultado final es una sola instancia activa.

La idea principal se puede resumir así:

> Aunque se intente crear varias veces el objeto, solamente uno permanece en el juego.

---

## 10. Posibles aplicaciones dentro del juego

Después de comprobar el funcionamiento del patrón, se pueden plantear aplicaciones dentro del proyecto, por ejemplo:

- Permitir solamente un proyectil especial activo al mismo tiempo.
- Permitir solamente un súper ataque activo.
- Crear una única plataforma especial.
- Mantener un único power-up especial dentro del escenario.
- Tener un solo gestor encargado de administrar las plataformas.
- Controlar una única configuración general de la partida.

Estas ideas corresponden a posibles aplicaciones futuras del patrón y no modifican la prueba principal realizada con `Inventory`.

---

## 11. Tecnologías utilizadas

- C++.
- Unreal Engine 4.27.2.
- Visual Studio.
- Git.
- GitHub.

---

## 12. Ejecución

1. Abrir `AventuraUSFX022026L4.uproject` con Unreal Engine 4.27.2.
2. Compilar el código C++.
3. Colocar `Singleton_Main` dentro del nivel.
4. Ejecutar el juego.
5. Observar los mensajes mostrados en pantalla.
6. Verificar en el `World Outliner` que solamente queda un `Inventory`.
7. Si se utiliza la representación visual, comprobar que solamente permanece una malla de `Inventory`.

---

## 13. Resultado

El patrón Singleton fue implementado correctamente.

Aunque `Singleton_Main` realiza varios intentos de creación, la clase `AInventory` detecta las instancias existentes y elimina las adicionales mediante `Destroy()`.

Como resultado, solamente permanece **una instancia de `Inventory`** durante la ejecución.

---

## 14. Repositorio

https://github.com/sebaslopezhurtado15/Lab-G4-PATRONES-Singleton
