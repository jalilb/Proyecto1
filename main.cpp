#include <iostream>
#include <fstream>
#include <list>
#include <stdlib.h>

using namespace std;

class Archivo{
public:
    string nombre_de_archivo;
    Archivo(string nombre_de_archivo){
        this->nombre_de_archivo=nombre_de_archivo;
    }
};

string ubicacion;

bool salir = true;

list<Archivo*>archivos;

void menu(){
    system("clear");
    cout<<"\tMenu\n"
    <<"1) Crea un archivo\n"
    <<"\ttouch nombre_de_archivo\n"
    <<"2) Lista todos los archivos creados\n"
    <<"\tls\n"
    <<"3) Modicar el contenido de un archivo\n"
    <<"\tedit  nombre_de_archivo 'contenido'\n"
    <<"4) Muestra el contenido de un archivo\n"
    <<"\tcat nombre_de_archivo\n"
    <<"5) Borrar archivo\n"
    <<"\trm nombre_de_archivo\n"
    <<"6) Copiar archivo\n"
    <<"\tcp nombre_de_archivo_fuente nombre_de_archivo_destino\n"
    <<"7) Crear carpeta\n"
    <<"\tmkdir nombre_de_carpeta\n"
    <<"8) Cambiar directorio\n"
    <<"\tcd nombre_de_carpeta\n"
    <<"9) help\n"
    <<"10) exit\n";
}

void leer_lista_en_archivo(){
    ifstream in("lista_de_archivos");
    string nombre_de_archivo;
    while(in>>nombre_de_archivo){
        archivos.push_back(new Archivo(nombre_de_archivo));
    }
}

void escribir_lista_en_archivo(){
    ofstream out("lista_de_archivos");
    for(list<Archivo*>::iterator i = archivos.begin(); i!=archivos.end(); i++){
        out<<(*i)->nombre_de_archivo<<" ";
    }
    out.close();
}

bool existe(string nombre_de_archivo){
    for(list<Archivo*>::iterator i = archivos.begin(); i!=archivos.end(); i++){
        if(nombre_de_archivo==(*i)->nombre_de_archivo){
            return true;
        }
    }
    return false;
}

void creararchivo(string nombre_de_archivo){
    if(!existe(nombre_de_archivo)){
        ofstream out(nombre_de_archivo.c_str());
        archivos.push_back(new Archivo(nombre_de_archivo));
        escribir_lista_en_archivo();
    }else{
        cout<<"Error: El archivo ya existe, usa un nuevo nombre :("<<endl;
    }
}

void listararchivos(){
    for(list<Archivo*>::iterator i = archivos.begin(); i!=archivos.end(); i++){
        cout<<(*i)->nombre_de_archivo<<endl;
    }
}

void modificararchivo(string nombre_de_archivo, string contenido){
    ofstream out(nombre_de_archivo.c_str());
    if(!out.is_open()){
        cout<<"Error: El archivo "<<nombre_de_archivo<<" no existe :("<<endl;
    }
    int tam=contenido.size()+1;
    out.write(contenido.c_str(),tam);
    out.close();
}

string verarchivo(string nombre_de_archivo){
    ifstream in(nombre_de_archivo.c_str());
    if(!in.is_open()){
        cout<<"Error: El archivo "<<nombre_de_archivo<<" no existe :("<<endl;
    }else{
        in.seekg(0,in.end);
        int tam = in.tellg();
        if(tam>0){
            in.seekg(0);
            char contenido[tam];
            in.read(contenido,tam);
            return contenido;
        }else{
            return"Error: El archivo esta vacio :( modificalo para que tenga informacion :D";
        }
    }
}

void borrararchivo(string nombre_de_archivo){
    bool se_borro=false;
    list<Archivo*>temp;
    for(list<Archivo*>::iterator i = archivos.begin(); i!=archivos.end(); i++){
        if(nombre_de_archivo!=(*i)->nombre_de_archivo){
            temp.push_back(new Archivo((*i)->nombre_de_archivo));
            }else{
                se_borro=true;
            }
    }
    if(se_borro){
        archivos = temp;
        escribir_lista_en_archivo();
        string borrar = "rm " + nombre_de_archivo;
        system(borrar.c_str());
        cout<<"Se borro exitosamente el archivo "<<nombre_de_archivo<<endl;
    }
}

void copiararchivo(string nombre_de_archivo_fuente,string nombre_de_archivo_destino){
    string contenido = verarchivo(nombre_de_archivo_fuente);
    if(contenido!="Error: El archivo esta vacio :( modificalo para que tenga informacion :D"){
        modificararchivo(nombre_de_archivo_destino,contenido);
    }else{
        cout<<"El archivo destino no existe"<<endl;
    }
}

void crearcarpeta(string nombre_de_carpeta){
    string crear = "mkdir "+nombre_de_carpeta;
    system(crear.c_str());
}

void Cambiardirectorio(string carpeta){
    string cambiar = "cd "+carpeta;
    system(cambiar.c_str());
    ubicacion=ubicacion+carpeta+"/";
}

bool iniciar(){
    string comando;
    cout<<ubicacion<<" > ";
    cin>>comando;
    if(comando=="touch"){
        string nombre_de_archivo;
        cin>>nombre_de_archivo;
        creararchivo(nombre_de_archivo);
        return true;
    }
    if(comando=="ls"){
        listararchivos();
        return true;
    }
    if(comando=="edit"){
        string nombre_de_archivo;
        string contenido;
        cin>>nombre_de_archivo;
        getline(cin, contenido);
        modificararchivo(nombre_de_archivo,contenido);
        return true;
    }
    if(comando=="cat"){
        string nombre_de_archivo;
        cin>>nombre_de_archivo;
        cout<<verarchivo(nombre_de_archivo)<<"\n";
        return true;
    }
    if(comando=="rm"){
        string nombre_de_archivo;
        cin>>nombre_de_archivo;
        borrararchivo(nombre_de_archivo);
        return true;
    }
    if(comando=="cp"){
        string nombre_de_archivo_fuente;
        cin>>nombre_de_archivo_fuente;
        string nombre_de_archivo_destino;
        cin>>nombre_de_archivo_destino;
        copiararchivo(nombre_de_archivo_fuente,nombre_de_archivo_destino);
        return true;
    }
    if(comando=="mkdir"){
        string nombre_de_carpeta;
        cin>>nombre_de_carpeta;
        crearcarpeta(nombre_de_carpeta);
        return false;
    }
    if(comando=="cd"){
        string carpeta;
        cin>>carpeta;
        if(carpeta == "/"){
            return false;
            salir=false;
        }
        Cambiardirectorio(carpeta);
        return true;
    }
    if(comando=="help"){
        menu();
        return true;
    }
    if(comando=="exit"){
        return false;
        salir=true;
    }
    cout<<"Error: Comando no reconocido  :("<<endl;
    return true;
}

int main()
{
    ubicacion="/";
    leer_lista_en_archivo();
    menu();
    while(iniciar());
    if(salir){
        return 0;
    }else{
        main();
    }
}
