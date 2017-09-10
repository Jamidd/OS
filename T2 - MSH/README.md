# Tarea 2 S.O Parte I. myShell

## Integrantes

* Jaime Middleton
* Fabio Traverso

## USO
```
$ make
$ ./msh

```

## Supuestos:

* Ctrl + c maneja salida de comando ejecutándose sólo si no se encuentra en modo paralelo (con '&' en último argumento), ya que se supone que se sigue encontrando en la shell básica.
* Sólo se guarda el exitCode en caso de que no se ejecute comando en modo paralelo (se pierde el proceso).
