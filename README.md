# PATRÓN FACTORY METHOD — SIS457 - Grupo 4

Proyecto desarrollado en **Unreal Engine 4.27.2** utilizando **C++**.

Este trabajo implementa el patrón de diseño **Factory Method** tomando como referencia el ejemplo **“Boil and Bubble, Brew and Bottle… Factory Method”** del libro *Unreal Engine 5 Game Programming Design Patterns in C++, Java, C#, and Blueprints*.

La implementación final fue adaptada al juego **AventuraUSFX022026L4**, utilizando la clase base `APlataforma` y dos tipos concretos de plataformas:

- `APlataformaIndestructible`
- `APlataformaDestructible`

El objetivo es centralizar la creación de plataformas dentro de un método fábrica. De esta forma, el `GameMode` ya no necesita decidir directamente qué clase concreta debe crear mediante llamadas separadas a `SpawnActor()`.

---

## 1. Objetivo

Aplicar el patrón **Factory Method** dentro del juego para centralizar la creación de diferentes tipos de plataformas.

Al iniciar la partida:

- El `GameMode` genera una cantidad aleatoria de plataformas.
- Se crean entre **5 y 20 plataformas**.
- Para cada plataforma se selecciona aleatoriamente un tipo.
- El `GameMode` llama al método fábrica `APlataforma::CrearPlataforma()`.
- El método fábrica decide qué clase concreta debe crear.
- Puede crear una `APlataformaIndestructible` o una `APlataformaDestructible`.
- El objeto creado se devuelve como un puntero de tipo `APlataforma*`.
- La plataforma creada se almacena en el `TArray` del `GameMode`.

La creación se realiza automáticamente desde `BeginPlay()`, por lo que no es necesario colocar manualmente las plataformas del patrón dentro del nivel.

---

## 2. Implementación final

Las clases principales relacionadas con el patrón son:

- `AAventuraUSFX022026L4GameMode`
- `APlataforma`
- `APlataformaIndestructible`
- `APlataformaDestructible`

La lógica principal del Factory Method se encuentra en:

```cpp
APlataforma::CrearPlataforma()
```

Antes de aplicar el patrón, el `GameMode` decidía directamente qué clase concreta crear utilizando un `if` y llamadas separadas a `SpawnActor()`.

La lógica anterior era conceptualmente:

```cpp
if (TipoAleatorio == 0)
{
    PlataformaActual =
        World->SpawnActor<APlataformaIndestructible>(
            SpawnLocation,
            Rotacion,
            Parametros
        );
}
else
{
    PlataformaActual =
        World->SpawnActor<APlataformaDestructible>(
            SpawnLocation,
            Rotacion,
            Parametros
        );
}
```

Con la implementación actual, el `GameMode` solamente solicita la creación:

```cpp
PlataformaActual = APlataforma::CrearPlataforma(
    World,
    TipoAleatorio,
    SpawnLocation,
    Rotacion
);
```

De esta manera, la decisión de qué objeto concreto crear queda concentrada en un solo método.

---

## 3. Clase Plataforma

`APlataforma` hereda de:

```cpp
AActor
```

Esto permite que cada plataforma exista físicamente dentro del nivel de Unreal Engine.

La clase base contiene la lógica general utilizada por las plataformas, por ejemplo:

- Malla de la plataforma.
- Posición.
- Dirección.
- Velocidad.
- Límites de movimiento.
- Configuración de movimiento.
- `BeginPlay()`.
- `Tick()`.
- Inicio y detención del movimiento.

Además, contiene el método utilizado como fábrica:

```cpp
static APlataforma* CrearPlataforma(
    UWorld* World,
    int Tipo,
    const FVector& Posicion,
    const FRotator& Rotacion
);
```

El tipo de retorno es:

```cpp
APlataforma*
```

Esto permite que el mismo método pueda devolver objetos de clases hijas de `APlataforma`.

---

## 4. Productos concretos

Los dos productos concretos utilizados actualmente por el método fábrica son:

### Plataforma Indestructible

```cpp
class APlataformaIndestructible : public APlataforma
```

Hereda de `APlataforma`.

Visualmente utiliza:

```cpp
TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'")
```

y el material:

```cpp
TEXT("Material'/Game/MaterialesPaintball/M_PlataformaAzul.M_PlataformaAzul'")
```

Por lo tanto, la plataforma indestructible se identifica mediante el material azul.

### Plataforma Destructible

```cpp
class APlataformaDestructible : public APlataforma
```

También hereda de `APlataforma`.

Utiliza la misma malla de cubo, pero con el material:

```cpp
TEXT("Material'/Game/MaterialesPaintball/M_PlataformaRoja.M_PlataformaRoja'")
```

La plataforma destructible se identifica mediante el material rojo.

Además, registra el evento:

```cpp
mallaPlataforma->OnComponentHit.AddDynamic(
    this,
    &APlataformaDestructible::AlRecibirImpacto
);
```

Cuando recibe un impacto de un componente cuyo perfil de colisión es `Projectile`, ejecuta:

```cpp
Destroy();
```

Por tanto:

- Azul → plataforma indestructible.
- Roja → plataforma destructible.

---

## 5. Método Factory Method

La lógica principal se encuentra dentro de:

```cpp
APlataforma* APlataforma::CrearPlataforma(
    UWorld* World,
    int Tipo,
    const FVector& Posicion,
    const FRotator& Rotacion
)
```

Primero se comprueba que exista un mundo válido:

```cpp
if (World == nullptr)
{
    return nullptr;
}
```

Después se crean los parámetros utilizados para generar el actor:

```cpp
FActorSpawnParameters Parametros;

Parametros.SpawnCollisionHandlingOverride =
    ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
```

Luego el método decide qué producto concreto crear.

Si:

```cpp
Tipo == 0
```

se crea:

```cpp
return World->SpawnActor<APlataformaIndestructible>(
    Posicion,
    Rotacion,
    Parametros
);
```

En caso contrario se crea:

```cpp
return World->SpawnActor<APlataformaDestructible>(
    Posicion,
    Rotacion,
    Parametros
);
```

El punto importante es que ambas clases se devuelven como:

```cpp
APlataforma*
```

porque ambas heredan de `APlataforma`.

---

## 6. Inicio automático desde GameMode

La creación de las plataformas se realiza dentro de:

```cpp
AAventuraUSFX022026L4GameMode::BeginPlay()
```

Primero se obtiene el mundo:

```cpp
UWorld* World = GetWorld();
```

y se comprueba:

```cpp
if (World == nullptr)
{
    return;
}
```

Después se prepara una lista de posiciones disponibles dentro del escenario:

```cpp
TArray<FVector> PosicionesDisponibles;
```

Las posiciones se generan utilizando dos ciclos:

```cpp
for (int X = 350; X <= 750; X += 130)
{
    for (int Y = -350; Y <= 350; Y += 175)
    {
        PosicionesDisponibles.Add(
            FVector(X, Y, 160.0f)
        );
    }
}
```

La cantidad de plataformas es aleatoria:

```cpp
int CantidadPlataformas = FMath::RandRange(5, 20);
```

El `TArray` principal reserva espacio para esa cantidad:

```cpp
aPlataformas.Reserve(CantidadPlataformas);
```

Después comienza la creación:

```cpp
for (int i = 0; i < CantidadPlataformas; i++)
```

Para cada plataforma se selecciona una posición aleatoria:

```cpp
int IndiceAleatorio =
    FMath::RandRange(0, PosicionesDisponibles.Num() - 1);

FVector SpawnLocation =
    PosicionesDisponibles[IndiceAleatorio];
```

También se selecciona aleatoriamente el tipo:

```cpp
int TipoAleatorio = FMath::RandRange(0, 1);
```

Finalmente, el `GameMode` llama al método fábrica:

```cpp
PlataformaActual = APlataforma::CrearPlataforma(
    World,
    TipoAleatorio,
    SpawnLocation,
    Rotacion
);
```

Si la plataforma fue creada correctamente:

```cpp
if (IsValid(PlataformaActual))
{
    aPlataformas.Add(PlataformaActual);
}
```

La posición utilizada se elimina del arreglo:

```cpp
PosicionesDisponibles.RemoveAt(IndiceAleatorio);
```

Esto evita utilizar nuevamente la misma posición durante esa generación.

---

## 7. Flujo general

```text
PLAY
 |
 v
GameMode::BeginPlay()
 |
 v
Genera posiciones disponibles
 |
 v
Selecciona entre 5 y 20 plataformas
 |
 v
Selecciona TipoAleatorio
 |
 v
APlataforma::CrearPlataforma()
 |
 +-----------------------------------+
 |                                   |
 v                                   v
Tipo == 0                        Tipo != 0
 |                                   |
 v                                   v
APlataformaIndestructible       APlataformaDestructible
 |                                   |
 +----------------+------------------+
                  |
                  v
            APlataforma*
                  |
                  v
       Se guarda en aPlataformas
```

Resultado:

```text
GameMode
   ↓
solicita una plataforma
   ↓
Factory Method
   ↓
decide el tipo concreto
   ↓
crea el objeto
   ↓
devuelve APlataforma*
```

---

## 8. Comprobación

Al ejecutar el proyecto se debe observar que aparecen entre **5 y 20 plataformas**.

Las plataformas pueden ser de dos tipos:

```text
Plataforma azul
→ APlataformaIndestructible

Plataforma roja
→ APlataformaDestructible
```

La selección se realiza de forma aleatoria mediante:

```cpp
FMath::RandRange(0, 1);
```

También se puede comprobar el comportamiento de la plataforma destructible.

Cuando recibe el impacto de un componente con perfil:

```cpp
Projectile
```

la plataforma roja ejecuta:

```cpp
Destroy();
```

La plataforma azul no contiene esa lógica de destrucción por impacto.

También se puede comprobar en el **World Outliner** que los actores creados corresponden a las clases concretas generadas por el método fábrica.

---

## 9. ¿Por qué se inicia desde GameMode?

La creación se realiza desde el `GameMode` porque:

- Su `BeginPlay()` se ejecuta automáticamente al comenzar la partida.
- Ya administra el arreglo de plataformas del proyecto.
- Permite generar todas las plataformas desde un punto central.
- Evita colocar manualmente cada plataforma dentro del mapa.
- El `GameMode` solicita objetos, pero la decisión concreta de creación se delega al método fábrica.

La responsabilidad queda separada de forma sencilla:

```text
GameMode
→ decide cuándo y dónde necesita una plataforma.

APlataforma::CrearPlataforma()
→ decide qué clase concreta crear.

APlataformaIndestructible / APlataformaDestructible
→ representan los productos concretos.
```

---

## 10. Archivos principales

```text
Source/AventuraUSFX022026L4/
│
├── AventuraUSFX022026L4GameMode.h
├── AventuraUSFX022026L4GameMode.cpp
├── Plataforma.h
├── Plataforma.cpp
├── PlataformaIndestructible.h
├── PlataformaIndestructible.cpp
├── PlataformaDestructible.h
└── PlataformaDestructible.cpp
```

### AventuraUSFX022026L4GameMode.h

Contiene el arreglo de plataformas:

```cpp
UPROPERTY()
TArray<APlataforma*> aPlataformas;
```

### AventuraUSFX022026L4GameMode.cpp

Contiene:

- `BeginPlay()`.
- Generación de posiciones.
- Cantidad aleatoria de plataformas.
- Selección aleatoria del tipo.
- Llamada a `APlataforma::CrearPlataforma()`.
- Almacenamiento de los objetos creados.

### Plataforma.h

Declara:

- La clase base `APlataforma`.
- Variables de movimiento.
- `ConfigurarMovimiento()`.
- `IniciarMovimiento()`.
- `DetenerMovimiento()`.
- El método estático `CrearPlataforma()`.

### Plataforma.cpp

Contiene:

- Constructor de la plataforma.
- Configuración de la malla.
- Movimiento de la plataforma.
- `BeginPlay()`.
- `Tick()`.
- Implementación de `CrearPlataforma()`.

### PlataformaIndestructible.h / .cpp

Contiene:

- Clase hija de `APlataforma`.
- Malla de cubo.
- Material azul.
- Configuración de colisión.

### PlataformaDestructible.h / .cpp

Contiene:

- Clase hija de `APlataforma`.
- Malla de cubo.
- Material rojo.
- Evento `OnComponentHit`.
- Método `AlRecibirImpacto()`.
- Destrucción cuando recibe un impacto de tipo `Projectile`.

---

## 11. Conceptos utilizados

### C++ y Programación Orientada a Objetos

- Clases.
- Objetos.
- Herencia.
- Clase padre.
- Clases hijas.
- Métodos.
- Métodos estáticos.
- Punteros.
- Parámetros.
- `return`.
- `if / else`.
- Polimorfismo mediante punteros a la clase base.
- Patrón Factory Method.

### Unreal Engine 4.27.2

- `AActor`.
- `AGameModeBase`.
- `BeginPlay()`.
- `Tick()`.
- `UWorld`.
- `GetWorld()`.
- `SpawnActor()`.
- `FActorSpawnParameters`.
- `ESpawnActorCollisionHandlingMethod::AlwaysSpawn`.
- `TArray`.
- `FVector`.
- `FRotator`.
- `FMath::RandRange()`.
- `IsValid()`.
- `Destroy()`.
- `UStaticMeshComponent`.
- `CreateDefaultSubobject()`.
- `ConstructorHelpers::FObjectFinder`.
- `OnComponentHit`.
- `AddDynamic()`.
- `UPROPERTY`.
- `UFUNCTION`.

---

## 12. Relación con el libro

El libro presenta Factory Method mediante un sistema de tiendas de pociones.

Su ejemplo utiliza conceptos como:

```text
Creator
Concrete Creator
Product
Concrete Product
```

La idea principal es que el código que solicita un objeto no tenga que encargarse directamente de todos los detalles de creación.

En el proyecto se conserva esa idea central:

```text
GameMode
      ↓
Solicita una plataforma
      ↓
APlataforma::CrearPlataforma()
      ↓
Decide qué producto concreto crear
      ↓
APlataformaIndestructible
            o
APlataformaDestructible
      ↓
Devuelve APlataforma*
```

La adaptación realizada es más sencilla que la estructura completa presentada en el libro.

En lugar de crear varias clases creadoras diferentes, se utiliza un método estático dentro de `APlataforma`:

```cpp
static APlataforma* CrearPlataforma(...);
```

Esto mantiene el código acorde al nivel del laboratorio y permite concentrar la decisión de creación en un único lugar.

Antes:

```text
GameMode
→ sabía qué clase concreta debía crear.
```

Después:

```text
GameMode
→ solicita una plataforma.

CrearPlataforma()
→ decide cuál crear.
```

---

## 13. Resultado

El patrón Factory Method quedó integrado al proyecto.

Al presionar **Play**, el `GameMode` genera automáticamente entre **5 y 20 plataformas**.

Para cada plataforma se selecciona aleatoriamente uno de los dos tipos disponibles y se llama:

```cpp
APlataforma::CrearPlataforma()
```

El método fábrica crea:

```text
Tipo 0
→ APlataformaIndestructible

Tipo 1
→ APlataformaDestructible
```

El objeto concreto se devuelve como:

```cpp
APlataforma*
```

y se almacena dentro de:

```cpp
TArray<APlataforma*> aPlataformas;
```

El resultado final es:

```text
El GameMode ya no crea directamente cada clase concreta.
                         ↓
La creación se concentra en CrearPlataforma().
                         ↓
El método fábrica selecciona el producto concreto.
                         ↓
El juego puede trabajar con ambos tipos mediante APlataforma*.
```

---

## 14. Tecnologías utilizadas

- Unreal Engine 4.27.2
- C++
- Visual Studio
- Git
- GitHub

---

## 15. Repositorio

https://github.com/sebaslopezhurtado15/Lab-G4-PATRONES-Factory-Method
