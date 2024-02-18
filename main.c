#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PACIENTES 250
#define MAX_CONSULTAS 1000  // Suponiendo que no haya más de 1000 consultas en total

// Definición de estructuras
typedef struct {
    int dia;
    int mes;
    int anio;
} TFecha;

typedef struct {
    int DNI;
    char Apellido[50];
    char Nombres[50];
    TFecha FNacimiento;
    int Telefono;
    char Sexo;
} Paciente;

typedef struct {
    int DNIPaciente;
    TFecha FConsulta;
    char Sintomas[100];
    char Tratamiento[100];
    char Control[20];
    char NombreMedico[50];
    char Especialidad[50];
} Consulta;

// Prototipos de funciones
void altaPaciente(Paciente pacientes[], int *numPacientes);
void eliminarPaciente(Paciente pacientes[], int *numPacientes);
void registrarConsulta(Paciente pacientes[], int numPacientes, Consulta consultas[], int *numConsultas);
void verHistoriaClinica(Paciente pacientes[], int numPacientes, Consulta consultas[], int numConsultas);
void consultasPorMedico(Consulta consultas[], int numConsultas, char nombreMedico[]);
void listadoChequeoPreventivo(Paciente pacientes[], int numPacientes);
TFecha obtenerFechaActual();
void cargarDatosDesdeArchivo(Paciente pacientes[], int *numPacientes, Consulta consultas[], int *numConsultas);
void guardarDatosEnArchivo(Paciente pacientes[], int numPacientes, Consulta consultas[], int numConsultas);
int buscarPacientePorDNI(Paciente pacientes[], int numPacientes, int dni);
int buscarConsultaPorDNIyFecha(Consulta consultas[], int numConsultas, int dni, TFecha fecha);
void mostrarPaciente(Paciente paciente);
void mostrarConsulta(Consulta consulta);

int main() {
    Paciente pacientes[MAX_PACIENTES];
    Consulta consultas[MAX_CONSULTAS];
    int numPacientes = 0;
    int numConsultas = 0;
    int opcion;

    // Cargar datos desde archivos (si existen)
    cargarDatosDesdeArchivo(pacientes, &numPacientes, consultas, &numConsultas);

    do {
        // Mostrar menú
        printf("\nMenú de opciones:\n");
        printf("1) Alta de un nuevo paciente\n");
        printf("2) Eliminar a un paciente\n");
        printf("3) Registrar una consulta\n");
        printf("4) Ver Historia Clínica\n");
        printf("5) Ver consultas atendidas por un médico\n");
        printf("6) Listado Chequeo Preventivo\n");
        printf("7) Guardar y Salir\n");

        printf("Ingrese su opción: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                altaPaciente(pacientes, &numPacientes);
                break;
            case 2:
                eliminarPaciente(pacientes, &numPacientes);
                break;
            case 3:
                registrarConsulta(pacientes, numPacientes, consultas, &numConsultas);
                break;
            case 4:
                verHistoriaClinica(pacientes, numPacientes, consultas, numConsultas);
                break;
            case 5: {
                char nombreMedico[50];
                printf("Ingrese el nombre del médico: ");
                scanf("%s", nombreMedico);
                consultasPorMedico(consultas, numConsultas, nombreMedico);
                break;
            }
            case 6:
                listadoChequeoPreventivo(pacientes, numPacientes);
                break;
            case 7:
                guardarDatosEnArchivo(pacientes, numPacientes, consultas, numConsultas);
                printf("Datos guardados. Saliendo del programa.\n");
                break;
            default:
                printf("Opción inválida. Por favor, ingrese un número válido.\n");
        }
    } while(opcion != 7);

    return 0;
}

void altaPaciente(Paciente pacientes[], int *numPacientes) {
    if (*numPacientes >= MAX_PACIENTES) {
        printf("Se ha alcanzado el límite máximo de pacientes. No se puede agregar más.\n");
        return;
    }

    Paciente nuevoPaciente;
    printf("Ingrese el DNI del paciente: ");
    scanf("%d", &nuevoPaciente.DNI);

    if (buscarPacientePorDNI(pacientes, *numPacientes, nuevoPaciente.DNI) != -1) {
        printf("Ya existe un paciente con ese DNI. No se puede agregar.\n");
        return;
    }

    printf("Ingrese el Apellido del paciente: ");
    scanf("%s", nuevoPaciente.Apellido);
    printf("Ingrese el Nombre del paciente: ");
    scanf("%s", nuevoPaciente.Nombres);
    printf("Ingrese la fecha de nacimiento del paciente (DD MM AAAA): ");
    scanf("%d %d %d", &nuevoPaciente.FNacimiento.dia, &nuevoPaciente.FNacimiento.mes, &nuevoPaciente.FNacimiento.anio);
    printf("Ingrese el teléfono del paciente: ");
    scanf("%d", &nuevoPaciente.Telefono);
    printf("Ingrese el sexo del paciente (M/F): ");
    scanf(" %c", &nuevoPaciente.Sexo);

    pacientes[*numPacientes] = nuevoPaciente;
    (*numPacientes)++;

    printf("Paciente agregado correctamente.\n");
}

void eliminarPaciente(Paciente pacientes[], int *numPacientes) {
    int dni;
    printf("Ingrese el DNI del paciente a eliminar: ");
    scanf("%d", &dni);

    int indice = buscarPacientePorDNI(pacientes, *numPacientes, dni);
    if (indice == -1) {
        printf("El DNI ingresado no pertenece a ningún paciente registrado.\n");
        return;
    }

    // Eliminar paciente moviendo los elementos posteriores
    for (int i = indice; i < *numPacientes - 1; ++i) {
        pacientes[i] = pacientes[i + 1];
    }
    (*numPacientes)--;

    printf("Paciente eliminado correctamente.\n");
}

void registrarConsulta(Paciente pacientes[], int numPacientes, Consulta consultas[], int *numConsultas) {
    if (*numConsultas >= MAX_CONSULTAS) {
        printf("Se ha alcanzado el límite máximo de consultas. No se puede registrar más.\n");
        return;
    }

    int dni;
    printf("Ingrese el DNI del paciente: ");
    scanf("%d", &dni);

    int indicePaciente = buscarPacientePorDNI(pacientes, numPacientes, dni);
    if (indicePaciente == -1) {
        printf("El DNI ingresado no pertenece a ningún paciente registrado.\n");
        return;
    }

    Consulta nuevaConsulta;
    nuevaConsulta.DNIPaciente = dni;
    nuevaConsulta.FConsulta = obtenerFechaActual();
    printf("Ingrese los síntomas: ");
    scanf("%s", nuevaConsulta.Sintomas);
    printf("Ingrese el tratamiento: ");
    scanf("%s", nuevaConsulta.Tratamiento);
    printf("Ingrese el control (cada X tiempo): ");
    scanf("%s", nuevaConsulta.Control);
    printf("Ingrese el nombre del médico: ");
    scanf("%s", nuevaConsulta.NombreMedico);
    printf("Ingrese la especialidad del médico: ");
    scanf("%s", nuevaConsulta.Especialidad);

    consultas[*numConsultas] = nuevaConsulta;
    (*numConsultas)++;

    printf("Consulta registrada correctamente.\n");
}

void verHistoriaClinica(Paciente pacientes[], int numPacientes, Consulta consultas[], int numConsultas) {
    int dni;
    printf("Ingrese el DNI del paciente: ");
    scanf("%d", &dni);

    int indicePaciente = buscarPacientePorDNI(pacientes, numPacientes, dni);
    if (indicePaciente == -1) {
        printf("El DNI ingresado no pertenece a ningún paciente registrado.\n");
        return;
    }

    printf("Historia Clínica del Paciente:\n");
    mostrarPaciente(pacientes[indicePaciente]);
    printf("Consultas:\n");

    for (int i = 0; i < numConsultas; ++i) {
        if (consultas[i].DNIPaciente == dni) {
            mostrarConsulta(consultas[i]);
        }
    }
}

void consultasPorMedico(Consulta consultas[], int numConsultas, char nombreMedico[]) {
    printf("Consultas atendidas por el médico %s:\n", nombreMedico);
    int encontradas = 0;

    for (int i = 0; i < numConsultas; ++i) {
        if (strcmp(consultas[i].NombreMedico, nombreMedico) == 0) {
            mostrarConsulta(consultas[i]);
            encontradas++;
        }
    }

    if (encontradas == 0) {
        printf("No se encontraron consultas atendidas por el médico %s.\n", nombreMedico);
    }
}

void listadoChequeoPreventivo(Paciente pacientes[], int numPacientes) {
    printf("Listado de Chequeo Preventivo (pacientes mayores de 40 años):\n");

    for (int i = 0; i < numPacientes; ++i) {
        if (obtenerFechaActual().anio - pacientes[i].FNacimiento.anio >= 40) {
            printf("Nombre: %s %s, Teléfono: %d\n", pacientes[i].Nombres, pacientes[i].Apellido, pacientes[i].Telefono);
        }
    }
}

TFecha obtenerFechaActual() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    TFecha fechaActual;
    fechaActual.dia = tm.tm_mday;
    fechaActual.mes = tm.tm_mon + 1;  // tm_mon va de 0 a 11
    fechaActual.anio = tm.tm_year + 1900;  // tm_year cuenta los años desde 1900

    return fechaActual;
}

void cargarDatosDesdeArchivo(Paciente pacientes[], int *numPacientes, Consulta consultas[], int *numConsultas) {
    FILE *archivoPacientes = fopen("pacientes.dat", "rb");
    FILE *archivoConsultas = fopen("consultas.dat", "rb");

    if (archivoPacientes != NULL && archivoConsultas != NULL) {
        fread(numPacientes, sizeof(int), 1, archivoPacientes);
        fread(pacientes, sizeof(Paciente), *numPacientes, archivoPacientes);

        fread(numConsultas, sizeof(int), 1, archivoConsultas);
        fread(consultas, sizeof(Consulta), *numConsultas, archivoConsultas);

        fclose(archivoPacientes);
        fclose(archivoConsultas);

        printf("Datos cargados desde los archivos.\n");
    } else {
        printf("No se encontraron archivos de datos existentes. Iniciando con datos vacíos.\n");
    }
}

void guardarDatosEnArchivo(Paciente pacientes[], int numPacientes, Consulta consultas[], int numConsultas) {
    FILE *archivoPacientes = fopen("pacientes.dat", "wb");
    FILE *archivoConsultas = fopen("consultas.dat", "wb");

    if (archivoPacientes != NULL && archivoConsultas != NULL) {
        fwrite(&numPacientes, sizeof(int), 1, archivoPacientes);
        fwrite(pacientes, sizeof(Paciente), numPacientes, archivoPacientes);

        fwrite(&numConsultas, sizeof(int), 1, archivoConsultas);
        fwrite(consultas, sizeof(Consulta), numConsultas, archivoConsultas);

        fclose(archivoPacientes);
        fclose(archivoConsultas);

        printf("Datos guardados en los archivos.\n");
    } else {
        printf("No se pudo guardar los datos en los archivos.\n");
    }
}

int buscarPacientePorDNI(Paciente pacientes[], int numPacientes, int dni) {
    for (int i = 0; i < numPacientes; ++i) {
        if (pacientes[i].DNI == dni) {
            return i;  // Se encontró el paciente en la posición i
        }
    }
    return -1;  // No se encontró el paciente
}

int buscarConsultaPorDNIyFecha(Consulta consultas[], int numConsultas, int dni, TFecha fecha) {
    for (int i = 0; i < numConsultas; ++i) {
        if (consultas[i].DNIPaciente == dni && consultas[i].FConsulta.dia == fecha.dia &&
            consultas[i].FConsulta.mes == fecha.mes && consultas[i].FConsulta.anio == fecha.anio) {
            return i;  // Se encontró la consulta en la posición i
        }
    }
    return -1;  // No se encontró la consulta
}

void mostrarPaciente(Paciente paciente) {
    printf("DNI: %d\n", paciente.DNI);
    printf("Nombre: %s %s\n", paciente.Nombres, paciente.Apellido);
    printf("Fecha de Nacimiento: %02d/%02d/%04d\n", paciente.FNacimiento.dia, paciente.FNacimiento.mes, paciente.FNacimiento.anio);
    printf("Teléfono: %d\n", paciente.Telefono);
    printf("Sexo: %c\n", paciente.Sexo);
}

void mostrarConsulta(Consulta consulta) {
    printf("Fecha de Consulta: %02d/%02d/%04d\n", consulta.FConsulta.dia, consulta.FConsulta.mes, consulta.FConsulta.anio);
    printf("Síntomas: %s\n", consulta.Sintomas);
    printf("Tratamiento: %s\n", consulta.Tratamiento);
    printf("Control: %s\n", consulta.Control);
    printf("Médico: %s\n", consulta.NombreMedico);
    printf("Especialidad: %s\n", consulta.Especialidad);
}
