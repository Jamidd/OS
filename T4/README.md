# Tarea 2 S.O Parte II. Life

## Integrantes

* Jaime Middleton
* Fabio Traverso

## USO
```
$ make
$ ./life
```

## CONSIDERACIONES
- para hacer una jugada es por ejemplo, 7b5bp, donde 7b es la posicion de origen (fil - 7, col - b), 5b es la posicion de destino (fil - 5, col - b), y p es el tipo de ficha
- los tipos de fichas son:
    # p -> peon
    # t -> torre
    # a -> alfil 
    # c -> caballo
    # R -> rey
    # r -> reina 
- al iniciar el juego, automaticamente estas dentro de la sala de chat, hasta que entras en estado waiting o empiezas a jugar con alguien