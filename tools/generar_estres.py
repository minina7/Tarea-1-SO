cantidad = 10000

with open("tests/estres_cadena.txt", "w") as archivo:

    for i in range(1, cantidad + 1):

        if i == 1:

            archivo.write(f"{i} : tarea{i} : 1 :\n")

        else:

            archivo.write(f"{i} : tarea{i} : 1 : {i - 1}\n")


with open("tests/estres_independientes.txt", "w") as archivo:

    for i in range(1, cantidad + 1):

        archivo.write(f"{i} : tarea{i} : 1 :\n")


print("Archivos de estres creados correctamente")