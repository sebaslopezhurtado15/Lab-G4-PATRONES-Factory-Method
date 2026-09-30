# PATRÓN SINGLETON — SIS457 - Grupo 4

Proyecto desarrollado en **Unreal Engine 4.27.2** utilizando **C++**.

Este trabajo implementa el patrón de diseño **Singleton** tomando como referencia el ejemplo **“Programming Elitists’ Bane… Singleton Pattern”** del libro *Unreal Engine 5 Game Programming Design Patterns in C++, Java, C#, and Blueprints*.

La implementación final fue adaptada al juego `AventuraUSFX022026L4` utilizando un **enemigo de tipo `ACharacter`**. El objetivo es que, aunque el juego intente crear varios enemigos, solamente permanezca **una instancia activa**.

---

## 1. Objetivo

Aplicar el patrón **Singleton** dentro del juego para garantizar que exista un solo enemigo.

Al iniciar la partida:

- El `GameMode` intenta crear varios enemigos.
- Cada enemigo comprueba si ya existe otro.
- El primer enemigo permanece.
- Los enemigos adicionales se destruyen.
- Al final solamente queda **un enemigo activo**.

La inicialización se realiza automáticamente desde el `GameMode`, por lo que ya no es necesario colocar manualmente un actor de prueba dentro del nivel.

---

## 2. Implementación final

Las clases principales utilizadas para el patrón son:

- `AAventuraUSFX022026L4GameMode`
- `AEnemigo`

La versión inicial del ejercicio se realizó siguiendo el ejemplo del libro con clases de prueba `Singleton_Main` e `Inventory`. Después de comprobar su funcionamiento, el patrón fue llevado al juego real utilizando `AEnemigo` y el `GameMode`.

Las clases de prueba ya no son necesarias en la implementación final.

---

## 3. Clase Enemigo

`AEnemigo` hereda de:

```cpp
ACharacter
```

Esto permite trabajar con el enemigo como un personaje de Unreal Engine.

La clase contiene una referencia a la instancia existente:

```cpp
UPROPERTY()
AEnemigo* Instance;
```

También contiene una malla para visualizar al enemigo:

```cpp
UPROPERTY(VisibleAnywhere)
class UStaticMeshComponent* MallaEnemigo;
```

---

## 4. Malla del enemigo

Para representar visualmente al enemigo se utiliza una malla del **Starter Content**:

```cpp
TEXT("/Game/StarterContent/Props/SM_Statue.SM_Statue")
```

La malla se conecta al `RootComponent` del `Character`:

```cpp
MallaEnemigo->SetupAttachment(RootComponent);
```

Además, se aumenta su escala:

```cpp
MallaEnemigo->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
```

De esta manera el enemigo se puede identificar fácilmente dentro del escenario.

---

## 5. Verificación Singleton

La lógica principal está dentro de:

```cpp
bool AEnemigo::VerificarSingleton()
```

Primero se buscan todos los enemigos existentes:

```cpp
TArray<AActor*> Instances;

UGameplayStatics::GetAllActorsOfClass(
    GetWorld(),
    AEnemigo::StaticClass(),
    Instances
);
```

Después se comprueba:

```cpp
if (Instances.Num() > 1)
```

Si existe más de un enemigo, significa que ya había una instancia válida.

Entonces se guarda la referencia al primer enemigo:

```cpp
Instance = Cast<AEnemigo>(Instances[0]);
```

y el nuevo enemigo se destruye:

```cpp
Destroy();
```

La función devuelve:

```cpp
return false;
```

cuando el nuevo enemigo debe eliminarse.

Si solamente existe una instancia, devuelve:

```cpp
return true;
```

y ese enemigo permanece en el juego.

---

## 6. Inicio automático desde GameMode

La creación de los enemigos se realiza dentro de:

```cpp
AAventuraUSFX022026L4GameMode::BeginPlay()
```

Esto permite que la prueba Singleton comience automáticamente al presionar **Play**.

El `GameMode` intenta crear cinco enemigos:

```cpp
for (int i = 0; i <= 4; i++)
```

Las posiciones se separan para permitir que los `ACharacter` puedan generarse correctamente:

```cpp
FVector PosicionEnemigo(
    200.0f,
    -200.0f + (i * 100.0f),
    200.0f
);
```

Después se crea cada enemigo con `SpawnActor()`.

Si el enemigo fue creado, se llama:

```cpp
SpawnedEnemigo->VerificarSingleton()
```

Solamente el enemigo que obtiene `true` se guarda como instancia válida:

```cpp
Enemigo = SpawnedEnemigo;
```

---

## 7. Flujo general

```text
PLAY
 |
 v
GameMode::BeginPlay()
 |
 v
Intenta crear 5 enemigos
 |
 v
Cada AEnemigo ejecuta VerificarSingleton()
 |
 +-------------------------------+
 |                               |
 v                               v
Primer enemigo              Ya existe otro
 |                               |
 v                               v
return true                 Guarda Instance
 |                               |
 v                               v
Permanece                    Destroy()
                                 |
                                 v
                            return false
```

Resultado final:

```text
5 intentos de creación
        ↓
1 enemigo permanece
        ↓
4 enemigos se destruyen
```

---

## 8. Comprobación

Durante la ejecución aparecen mensajes similares a:

```text
Enemigo_0 has been created
Enemigo_0 already exists
Enemigo_0 already exists
Enemigo_0 already exists
Enemigo_0 already exists
```

Esto demuestra que:

1. El primer enemigo fue creado.
2. Los demás intentos detectaron que el enemigo ya existía.
3. Las instancias adicionales fueron destruidas.
4. Al final queda solamente un enemigo visible.

También se puede comprobar en el **World Outliner**, donde debe existir una sola instancia de `AEnemigo`.

---

## 9. ¿Por qué se inicia desde GameMode?

Inicialmente la prueba se ejecutaba colocando manualmente un actor en el nivel.

En la implementación final se utiliza el `GameMode` porque:

- Su `BeginPlay()` se ejecuta automáticamente al comenzar la partida.
- Ya forma parte del flujo principal del proyecto.
- No es necesario colocar manualmente un objeto Singleton en el mapa.
- Permite iniciar la creación de enemigos directamente al presionar **Play**.

La responsabilidad queda separada de forma sencilla:

```text
GameMode
→ inicia la creación de enemigos.

AEnemigo
→ comprueba el Singleton y evita duplicados.
```

---

## 10. Archivos principales

```text
Source/AventuraUSFX022026L4/
│
├── AventuraUSFX022026L4GameMode.h
├── AventuraUSFX022026L4GameMode.cpp
├── Enemigo.h
└── Enemigo.cpp
```

### `AventuraUSFX022026L4GameMode.h`

Contiene la referencia:

```cpp
UPROPERTY()
AEnemigo* Enemigo;
```

### `AventuraUSFX022026L4GameMode.cpp`

Contiene la creación automática de los enemigos dentro de `BeginPlay()`.

### `Enemigo.h`

Declara:

- `MallaEnemigo`.
- `Instance`.
- `VerificarSingleton()`.

### `Enemigo.cpp`

Contiene:

- Configuración de la malla.
- Escala del enemigo.
- Búsqueda de enemigos existentes.
- Validación del Singleton.
- `Destroy()` de las instancias adicionales.

---

## 11. Conceptos utilizados

### C++ y Programación Orientada a Objetos

- Clases.
- Objetos.
- Herencia.
- Punteros.
- Métodos.
- `bool`.
- `TArray`.
- `Cast`.
- Patrón Singleton.

### Unreal Engine 4.27.2

- `ACharacter`.
- `AGameModeBase`.
- `BeginPlay()`.
- `SpawnActor()`.
- `GetWorld()`.
- `GetAllActorsOfClass()`.
- `Destroy()`.
- `UPROPERTY`.
- `UStaticMeshComponent`.
- `CreateDefaultSubobject()`.
- `ConstructorHelpers::FObjectFinder`.
- `GEngine->AddOnScreenDebugMessage()`.

---

## 12. Relación con el libro

El ejemplo del libro utiliza una clase que busca otras instancias de su mismo tipo y elimina las adicionales.

En el proyecto se conserva la misma idea:

```text
Buscar instancias
      ↓
¿Hay más de una?
      ↓
Sí
      ↓
Guardar la primera
      ↓
Destruir la nueva
```

La adaptación realizada consiste en aplicar esa lógica a un enemigo que hereda de `ACharacter` y hacer que la creación se inicie automáticamente desde el `GameMode`.

---

## 13. Resultado

El patrón Singleton quedó integrado al juego.

Al presionar **Play**, el `GameMode` intenta crear cinco enemigos, pero la clase `AEnemigo` verifica las instancias existentes y destruye las adicionales.

El resultado final es:

> **Aunque se intente crear varios enemigos, solamente permanece uno dentro del juego.**

---

## 14. Tecnologías utilizadas

- Unreal Engine 4.27.2
- C++
- Visual Studio
- Git
- GitHub

---

## 15. Repositorio

https://github.com/sebaslopezhurtado15/Lab-G4-PATRONES-Singleton
