# PacmanSPF

PacmanSPF is a small personal project which intends to help me learning CPP, playing around with graphs and using boost library.
It generates a graph which represents the different packages on my pacman database along with their dependencies.
The application offers to return a list of necessary or/and optional packages (ie. dependencies) to install a very list of packages - and only them.
Example : if take "avahi", the application will return its required or/and optional dependencies (required: gdbm, expat... ; optional : qt4, gtk2...).

## Features

* Generate a graph in .dot file format (Graphiz)
* Use pacman's library: alpm
* Use DFS
* Return required & optional dependencies of a list of packages

## Screenshot of the graph generated with Graphiz (and displayed with KGraphViewer)

![Graph diagram generation](images/graph.png)
