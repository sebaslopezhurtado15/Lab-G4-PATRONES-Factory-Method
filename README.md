# PATRÓN FACTORY METHOD — SIS457 - Grupo 4

Proyecto desarrollado en **Unreal Engine 4.27.2** utilizando **C++**.

Este trabajo implementa el patrón de diseño **Factory Method** dentro del proyecto **AventuraUSFX022026L4**.

La implementación actual separa la responsabilidad de crear plataformas del `GameMode` mediante una clase específica llamada `AFabricaPlataformas`.

El `GameMode` ya no decide directamente qué clase concreta debe crear. En su lugar, crea una instancia de la fábrica y solicita las plataformas mediante:

```cpp
Fabrica->CrearPlataforma(TipoAleatorio, SpawnLocation, Rotacion);
```

De esta manera, la lógica de creación queda concentrada en la fábrica y el `GameMode` solamente se encarga de solicitar y almacenar las plataformas creadas.

---

## 1. Objetivo

Aplicar el patrón **Factory Method** dentro del juego para separar la creación de plataformas de la lógica principal del `GameMode`.

Al iniciar la partida:

- El `GameMode` obtiene el `UWorld`.
- Se generan posiciones disponibles dentro del escenario.
- Se selecciona una cantidad aleatoria de plataformas entre **5 y 20**.
- Se crea una instancia de `AFabricaPlataformas`.
- Para cada plataforma se selecciona un tipo aleatorio.
- El `GameMode` llama al método `CrearPlataforma()`.
- La fábrica se encarga de crear la plataforma correspondiente.
- La plataforma creada se devuelve como `APlataforma*`.
- El objeto válido se almacena dentro del arreglo `aPlataformas`.

La creación se inicia automáticamente desde `BeginPlay()`.

---

## 2. Implementación final

Las clases principales utilizadas para aplicar Factory Method son:

```text
AAventuraUSFX022026L4GameMode
AFabricaPlataformas
APlataforma
```

También forman parte del sistema las clases concretas de plataformas existentes en el proyecto:

```text
APlataformaTerrestre
APlataformaAerea
APlataformaSubterranea
APlataformaAcuatica
APlataformaIndestructible
APlataformaDestructible
```

La modificación más importante de esta versión fue crear los archivos:

```text
FabricaPlataformas.h
FabricaPlataformas.cpp
```

Antes, la lógica de creación estaba directamente dentro del `GameMode`.

Ahora el flujo principal es:

```text
GameMode
   ↓
FabricaPlataformas
   ↓
CrearPlataforma()
   ↓
APlataforma*
```

Esto permite que el `GameMode` no tenga que encargarse directamente de decidir cómo se construye cada plataforma.

---

## 3. Clase base Plataforma

La clase:

```cpp
APlataforma
```

hereda de:

```cpp
AActor
```

Esto permite que cada plataforma exista como un actor dentro del nivel de Unreal Engine.

La clase base contiene elementos comunes utilizados por las plataformas:

```cpp
UStaticMeshComponent* mallaPlataforma;
```

También contiene variables relacionadas con el movimiento:

```cpp
FVector movimientoEjes;
FVector movimientoLimitesMaximos;
FVector movimientoLimitesMinimos;
FVector movimientoVelocidades;
FVector movimientoDireccion;
FVector posicionActual;
FVector posicionInicial;
```

Además utiliza:

```cpp
bool bMover;
```

para controlar si la plataforma se encuentra en movimiento.

La clase mantiene funciones como:

```cpp
void ConfigurarMovimiento(int Tipo);
void IniciarMovimiento();
void DetenerMovimiento();
```

y sobrescribe:

```cpp
virtual void Tick(float DeltaTime) override;
virtual void BeginPlay() override;
```

---

## 4. Tipo de plataforma

En `Plataforma.h` se utiliza:

```cpp
enum class ETipoPlataforma
{
    PLATAFORMA_TERRESTRE,
    PLATAFORMA_AEREA,
    PLATAFORMA_SUBTERRANEA,
    PLATAFORMA_ACUATICA,
};
```

Cada objeto `APlataforma` puede guardar su tipo mediante:

```cpp
ETipoPlataforma TipoPlataforma;
```

Esto permite identificar a qué grupo pertenece una plataforma.

Los tipos definidos son:

```text
PLATAFORMA_TERRESTRE
PLATAFORMA_AEREA
PLATAFORMA_SUBTERRANEA
PLATAFORMA_ACUATICA
```

Esta información también puede ser utilizada posteriormente por el `GameMode`, por ejemplo para comprobar el tipo de una plataforma y reposicionarla.

---

## 5. Clase FabricaPlataformas

La nueva clase:

```cpp
AFabricaPlataformas
```

representa la fábrica utilizada en la implementación actual del patrón.

Su responsabilidad es centralizar la creación de plataformas.

El `GameMode` no crea directamente cada plataforma concreta.

En su lugar llama:

```cpp
PlataformaActual = Fabrica->CrearPlataforma(
    TipoAleatorio,
    SpawnLocation,
    Rotacion
);
```

La fábrica recibe la información necesaria para realizar la creación:

```text
Tipo de plataforma
Posición
Rotación
```

y devuelve:

```cpp
APlataforma*
```

Esto permite trabajar desde el `GameMode` utilizando el tipo base `APlataforma`, independientemente de la clase concreta que haya sido creada.

---

## 6. Creación de la fábrica

Dentro de:

```cpp
AAventuraUSFX022026L4GameMode::BeginPlay()
```

primero se obtiene el mundo:

```cpp
UWorld* World = GetWorld();
```

Se verifica que sea válido:

```cpp
if (World == nullptr)
{
    return;
}
```

Después se preparan los parámetros de creación:

```cpp
FActorSpawnParameters Parametros;

Parametros.SpawnCollisionHandlingOverride =
    ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
```

Luego se crea una instancia de la fábrica:

```cpp
AFabricaPlataformas* Fabrica =
    World->SpawnActor<AFabricaPlataformas>(
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        Parametros
    );
```

La fábrica queda disponible para crear las plataformas durante el resto de `BeginPlay()`.

---

## 7. Generación de posiciones

Antes de crear las plataformas, el `GameMode` genera posiciones disponibles dentro del escenario.

Se utiliza:

```cpp
TArray<FVector> PosicionesDisponibles;
```

Las posiciones se generan con dos ciclos:

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

Cada posición representa un lugar posible para crear una plataforma.

Después de utilizar una posición, se elimina del arreglo:

```cpp
PosicionesDisponibles.RemoveAt(IndiceAleatorio);
```

Esto evita reutilizar la misma posición dentro de esa generación.

---

## 8. Cantidad aleatoria de plataformas

La cantidad de plataformas se determina con:

```cpp
int CantidadPlataformas = FMath::RandRange(5, 20);
```

Por lo tanto, al comenzar la partida se crean entre:

```text
5 y 20 plataformas
```

Antes de comenzar la creación, el arreglo reserva espacio:

```cpp
aPlataformas.Reserve(CantidadPlataformas);
```

La variable utilizada para recibir cada plataforma es:

```cpp
APlataforma* PlataformaActual = nullptr;
```

---

## 9. Uso del Factory Method

La creación se realiza dentro del ciclo:

```cpp
for (int i = 0; i < CantidadPlataformas; i++)
```

Primero se selecciona una posición aleatoria:

```cpp
int IndiceAleatorio =
    FMath::RandRange(0, PosicionesDisponibles.Num() - 1);

FVector SpawnLocation =
    PosicionesDisponibles[IndiceAleatorio];
```

Después se selecciona el tipo:

```cpp
int TipoAleatorio = FMath::RandRange(0, 1);
```

Finalmente se solicita una plataforma a la fábrica:

```cpp
PlataformaActual = Fabrica->CrearPlataforma(
    TipoAleatorio,
    SpawnLocation,
    Rotacion
);
```

El `GameMode` recibe el resultado mediante un puntero de tipo:

```cpp
APlataforma*
```

Si el objeto fue creado correctamente:

```cpp
if (IsValid(PlataformaActual))
{
    aPlataformas.Add(PlataformaActual);
}
```

La plataforma se almacena dentro de:

```cpp
TArray<APlataforma*> aPlataformas;
```

---

## 10. Diferencia con la implementación anterior

Antes de aplicar correctamente la separación mediante la fábrica, la decisión de qué plataforma crear estaba dentro del propio `GameMode`.

La lógica anterior utilizaba directamente diferentes llamadas a:

```cpp
World->SpawnActor<...>()
```

dependiendo del valor seleccionado.

Esto provocaba que el `GameMode` conociera directamente las clases concretas que debía crear.

La versión actual cambia ese flujo.

### Antes

```text
GameMode
   ↓
elige el tipo
   ↓
SpawnActor de la clase concreta
```

### Ahora

```text
GameMode
   ↓
elige un valor de tipo
   ↓
Fabrica->CrearPlataforma()
   ↓
la fábrica realiza la creación
   ↓
devuelve APlataforma*
```

La diferencia principal es que la responsabilidad de creación fue trasladada a:

```cpp
AFabricaPlataformas
```

---

## 11. Flujo general

```text
PLAY
 |
 v
GameMode::BeginPlay()
 |
 v
GetWorld()
 |
 v
Genera posiciones disponibles
 |
 v
Selecciona entre 5 y 20 plataformas
 |
 v
Crea AFabricaPlataformas
 |
 v
for
 |
 v
Selecciona posición aleatoria
 |
 v
Selecciona TipoAleatorio
 |
 v
Fabrica->CrearPlataforma()
 |
 v
Devuelve APlataforma*
 |
 v
IsValid()
 |
 v
aPlataformas.Add()
```

La idea principal del patrón queda resumida de esta forma:

```text
El GameMode solicita.
La fábrica crea.
El GameMode recibe el producto.
```

---

## 12. ¿Por qué usar una clase FabricaPlataformas?

La nueva implementación separa dos responsabilidades.

### GameMode

Se encarga de:

```text
Iniciar el proceso.
Calcular posiciones.
Elegir cantidades.
Solicitar plataformas.
Guardar las plataformas creadas.
```

### FabricaPlataformas

Se encarga de:

```text
Recibir la solicitud.
Decidir la creación correspondiente.
Crear el objeto.
Devolverlo como APlataforma*.
```

Esto evita colocar toda la lógica de creación dentro del `GameMode`.

La separación también hace más fácil explicar el patrón:

```text
El cliente no crea directamente el producto concreto.
Le pide a una fábrica que lo cree.
```

---

## 13. Polimorfismo utilizado

El método de la fábrica devuelve:

```cpp
APlataforma*
```

Esto es importante porque `APlataforma` funciona como clase base.

Las clases concretas de plataforma heredan de ella.

Por tanto, el `GameMode` puede almacenar diferentes plataformas dentro de un mismo arreglo:

```cpp
TArray<APlataforma*> aPlataformas;
```

El `GameMode` no necesita un arreglo diferente para cada clase concreta.

Todos los objetos pueden manejarse mediante punteros a:

```cpp
APlataforma
```

Este comportamiento se relaciona con el polimorfismo de la Programación Orientada a Objetos.

---

## 14. Inicio automático desde GameMode

La prueba comienza desde:

```cpp
AAventuraUSFX022026L4GameMode::BeginPlay()
```

Esto permite que al presionar **Play**:

```text
1. Comience la partida.
2. Se ejecute BeginPlay().
3. Se cree la fábrica.
4. Se generen las plataformas.
5. Se almacenen en aPlataformas.
```

No es necesario colocar manualmente una fábrica desde el editor.

La creación del actor fábrica se realiza mediante C++.

---

## 15. Relación con las plataformas existentes

El proyecto conserva las clases de plataformas desarrolladas anteriormente:

```text
PlataformaTerrestre
PlataformaAerea
PlataformaSubterranea
PlataformaAcuatica
PlataformaIndestructible
PlataformaDestructible
```

Todas forman parte de la estructura general de plataformas del proyecto.

La clase base continúa siendo:

```cpp
APlataforma
```

Esto permite mantener la lógica anterior del proyecto y agregar Factory Method sin reemplazar innecesariamente el sistema que ya funcionaba.

---

## 16. Movimiento de plataformas

`APlataforma` mantiene la lógica de movimiento desarrollada anteriormente.

Entre las funciones utilizadas están:

```cpp
ConfigurarMovimiento()
IniciarMovimiento()
DetenerMovimiento()
Tick()
```

El `GameMode` también mantiene funciones relacionadas con las plataformas:

```cpp
IniciarMovimiento()
EliminarUnaPlataformaPorHija()
ReposicionarPlataformas()
```

Estas funciones pertenecen a la lógica existente del proyecto y son independientes de la responsabilidad principal del Factory Method.

Factory Method se concentra específicamente en:

```text
LA CREACIÓN DE LOS OBJETOS.
```

---

## 17. Comprobación

Para comprobar la implementación:

1. Ejecutar el proyecto en Unreal Engine 4.27.2.
2. Presionar **Play**.
3. Verificar que se crean plataformas automáticamente.
4. Comprobar que la cantidad generada cambia entre ejecuciones.
5. Verificar que se generan entre **5 y 20 plataformas**.
6. Revisar el **World Outliner** para observar los actores creados.
7. Confirmar que el `GameMode` llama a:

```cpp
Fabrica->CrearPlataforma(...)
```

en lugar de encargarse directamente de todas las llamadas de creación concretas.

El resultado esperado es:

```text
El GameMode solicita las plataformas
        ↓
AFabricaPlataformas realiza la creación
        ↓
se devuelve APlataforma*
        ↓
las plataformas se almacenan en aPlataformas
```

---

## 18. Archivos principales

```text
Source/AventuraUSFX022026L4/
│
├── AventuraUSFX022026L4GameMode.h
├── AventuraUSFX022026L4GameMode.cpp
│
├── FabricaPlataformas.h
├── FabricaPlataformas.cpp
│
├── Plataforma.h
├── Plataforma.cpp
│
├── PlataformaTerrestre.h
├── PlataformaTerrestre.cpp
│
├── PlataformaAerea.h
├── PlataformaAerea.cpp
│
├── PlataformaSubterranea.h
├── PlataformaSubterranea.cpp
│
├── PlataformaAcuatica.h
├── PlataformaAcuatica.cpp
│
├── PlataformaIndestructible.h
├── PlataformaIndestructible.cpp
│
├── PlataformaDestructible.h
└── PlataformaDestructible.cpp
```

### AventuraUSFX022026L4GameMode.cpp

Contiene:

```text
BeginPlay().
Generación de posiciones.
Cantidad aleatoria.
Creación de AFabricaPlataformas.
Selección de TipoAleatorio.
Llamada a CrearPlataforma().
Almacenamiento en aPlataformas.
```

### FabricaPlataformas.h

Declara la clase fábrica utilizada para centralizar la creación de plataformas.

### FabricaPlataformas.cpp

Contiene la implementación de la lógica utilizada por la fábrica para crear las plataformas solicitadas.

### Plataforma.h

Contiene:

```text
Clase base APlataforma.
ETipoPlataforma.
Malla.
Variables de movimiento.
ConfigurarMovimiento().
IniciarMovimiento().
DetenerMovimiento().
```

### Plataforma.cpp

Contiene la implementación general del comportamiento de las plataformas.

---

## 19. Conceptos utilizados

### C++ y Programación Orientada a Objetos

- Clases.
- Objetos.
- Clase base.
- Clases derivadas.
- Herencia.
- Punteros.
- Métodos.
- Polimorfismo.
- Enumeraciones.
- `if`.
- `for`.
- `return`.
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
- `UStaticMeshComponent`.
- `UPROPERTY`.
- `Destroy()`.
- `SetActorLocation()`.

---

## 20. Relación con Factory Method

Factory Method es un patrón de creación.

Su objetivo es evitar que el código cliente tenga que encargarse directamente de todos los detalles relacionados con la creación de productos concretos.

En este proyecto:

```text
CLIENTE
AAventuraUSFX022026L4GameMode

        ↓ solicita

FÁBRICA
AFabricaPlataformas

        ↓ crea

PRODUCTO BASE
APlataforma
```

El `GameMode` trabaja con:

```cpp
APlataforma*
```

y solicita la creación utilizando:

```cpp
Fabrica->CrearPlataforma(...)
```

La responsabilidad de fabricar el objeto deja de estar directamente dentro del cliente.

---

## 21. Resultado

El patrón Factory Method quedó integrado al proyecto mediante una clase fábrica independiente.

La implementación actual funciona de la siguiente forma:

```text
GameMode
   ↓
crea AFabricaPlataformas
   ↓
selecciona un tipo
   ↓
llama CrearPlataforma()
   ↓
la fábrica crea el producto
   ↓
devuelve APlataforma*
   ↓
GameMode guarda el objeto
```

Al presionar **Play**, se crean automáticamente entre **5 y 20 plataformas** utilizando el nuevo sistema.

El cambio principal respecto a la versión anterior es:

```text
ANTES:
la creación estaba directamente en el GameMode.

AHORA:
la creación está delegada a AFabricaPlataformas.
```

Esto hace que la implementación represente de forma más clara la idea del patrón Factory Method.

---

## 22. Tecnologías utilizadas

- Unreal Engine 4.27.2
- C++
- Visual Studio
- Git
- GitHub

---

## 23. Repositorio

https://github.com/sebaslopezhurtado15/Lab-G4-PATRONES-Factory-Method
