#include <bits/stdc++.h>
#include <iostream>

using namespace std;

// estructura de los nodos del árbol
struct Node
{
    // Nodo
    char data;
    long id;
    int freq;
    bool isdummy = true;          // bandera y saber en que nodo está
    shared_ptr<Node> left, right; // derecho e izquierdo

    Node(char data, long id)
    {
        this->data = data;
        this -> id = id;
        isdummy = false;
    }
    Node(shared_ptr<Node> left, shared_ptr<Node> right, long id) : left(left), right(right)
    {
        // apuntra izquierdo derecho
        this -> left = left;
        this -> right = right;
        this -> id = id;
    }
};





string S; //pila
unordered_map<char, string> huffmanCodes;

void huffmanEncoding(shared_ptr<Node> father)
{
    if(father->isdummy == false)
    {
        //estamos en una hoja
        //creamos una asignacion del codigo huffman  por un char

        huffmanCodes[father->data] = S;
        return; //salimos de la recursión

    }
    //codigo hijo derecho
    S+="0";
    huffmanEncoding(father->left);
    S.pop_back();

    //codigo hijo izquierdo
    S+="1";
    huffmanEncoding(father->right);
    S.pop_back();
}

//convierte un string de 8 bits a un entero sin signo
unsigned int bitsToUnsignedInt(string& sbits)
{
    bitset<8>bits(sbits);
    unsigned int number = bits.to_ulong();
    return number;
}

int main()
{
    std::cin.tie(0);
    std::ios_base::sync_with_stdio(0);

    unordered_map<char, int> frequencies;
    int n;
    cin >> n;
    string text; //"COMO COME COCORITO COME COMO COSMONAUTA";
    string espacio = "";
    for(int i = 0; i<n; i++){
        cin >> espacio;
        if(i<n-1){
            text+=espacio+" ";
        }else{
            text+=espacio;
        }
    }

    for (auto c : text)
    {
        frequencies[c]++;
    }

    //VAMOS A PASAR LOS CARACTERES A UN VECTOR
    vector<char> orderedChars;
    for (auto entry : frequencies) //iteramos sobre el mapa
    {
        orderedChars.push_back(entry.first); //insertamos cada caracter que está en el mapa.
    }
    sort(orderedChars.begin(), orderedChars.end()); //ordenamos el vector desde pequeño al más grande


    //CREAMOS LA PRIORITY QUEUE CON NUESTROS NODOS HOJAS
    auto cmpNodes = [](shared_ptr<Node> &n1, shared_ptr<Node> &n2)
    {

        if (n1 -> freq == n2 -> freq) //si las fecuencias son iguales
        {
            return n1->id > n2->id; //comparamos los id
        }

        return n1->freq > n2->freq;
    };

    priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, decltype(cmpNodes)> Q(cmpNodes); // cola prioritaria
    long id = 0;
    for (char c : orderedChars) //iteramos el vector ordenado
    {
        auto node = make_shared<Node>(c, id); //creamos el nodo
        node->freq = frequencies[c];
        Q.push(node); //añadimos el nodo
        id++;
    }



    // creamos el arbol
    while (Q.size() > 1) // sacamos los subarboles de mayor frecuencia
    {
        auto left = Q.top();  // izquierdo hijo
        Q.pop();              // se saca de la cola
        auto right = Q.top(); // hijo derecho
        Q.pop();              // se saca de la cola

        // creamos el dummy
        auto dummyNode = make_shared<Node>(left, right, id);
        dummyNode->freq = left->freq + right->freq;

        // Lo reinsertamos en la cola
        Q.push(dummyNode);
        id++;
    }
    //obtenemos la raíz
    auto root = Q.top();
    string codificacion = "";

    //imprimimos la codificacion huffman
    huffmanEncoding(root);



    for (char c : text)
    {
        for(auto entry : huffmanCodes)
        {
            if(entry.first == c)
            {
                codificacion += entry.second;


                break;
            }
        }

    }

    int cont = codificacion.size() % 8;
    if(cont > 0)
    {
        cont = 8 - cont;
        codificacion += string(cont, '0');
    }


    string converter = "" ;




    int i = 0;
    while(i<codificacion.size())
    {
        converter+= codificacion[i];
        if (converter.size()== 8 )
        {
            int  ascii = bitsToUnsignedInt(converter);


            if((ascii>='A' && ascii<='Z') || (ascii>='a' && ascii<='z') )
            {
                cout<<(char)ascii;
            }
            else
            {
                cout << ascii;
            }

            if (i != codificacion.size() - 1)
            {
                cout << "-";
            }
            converter = "";
        }
        i++;
    }
    int tamtext = text.size();
    int tamcod = codificacion.size();

    int tambits = tamtext * 8;
    float compresion = (1 - (float)tamcod/tambits) * 100;
    cout << "\n" << (int)compresion;




    return 0;
}
