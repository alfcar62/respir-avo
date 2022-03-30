# unicsv
unicsv è un programma work-in-progress scritto in C che permette di unire due file CSV.

<div align="center">
  <img src="../.github/unicsv.PNG", width="800px" style="left: auto; right: auto">
</div>


## Compilazione
### Requisiti
* Un C Compiler (GCC/Clang)
* Make (mingw32-make/make)
### Procedura
```sh
make setup
make
```

## Esecuzione
Usare il comando
```sh
make run fp=posizioni.csv fm=misure.csv fo=out.csv
```
