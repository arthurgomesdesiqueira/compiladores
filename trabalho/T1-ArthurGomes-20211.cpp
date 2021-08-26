#include <bits/stdc++.h>

using namespace std;

//typedef pair<string, string> pss;
//typedef pair<string, pss> spss;

struct identificador
{
	string classe;
	string lexema;
	string tipo;

	identificador(string aux_classe, string aux_lexema, string aux_tipo)
    {
        classe = aux_classe;
		lexema = aux_lexema;
		tipo = aux_tipo;
    }

};

typedef struct identificador IDENTIFICADOR;

//variaveis globais
vector<IDENTIFICADOR> PALAVRAS_RESERVADAS;
unordered_map <string, string> ERROR;
unordered_set <char> DIGITO;
unordered_set <char> LETRA;


//Imprimir o token
void imprimir_token(string token, string lexema, string tipo)
{
	cout << "Classe: " << token << ", Lexema: " << lexema << ", Tipo: " << tipo << endl;
}

//Exemplo: ERRO2 – Missing double quotation mark , linha 1, coluna 7
//Imprimir o erro
void imprimir_erro(string tipo_erro, string mensagem, string linha, string coluna)
{
	cout << tipo_erro << " - " << mensagem << " , linha " << linha << ", coluna " << coluna << endl;
}

//NUM
int token_Num(string str, int pos, int linha)
{
	string lexema = "";
	bool deu_certo = true;

	if(DIGITO.find(str[pos]) != DIGITO.end())
	{
		lexema = lexema + str[pos];
		pos++; 

		while(pos < str.size() && DIGITO.find(str[pos]) != DIGITO.end())
		{
			lexema = lexema + str[pos];
			pos++; 
		}

		bool achou_real = false;
		if(pos < str.size() && (str[pos] == '.' || str[pos] == 'E' || str[pos] == 'e'))
		{
			if(str[pos] == '.')
			{
				achou_real = true;
				lexema = lexema + str[pos];
				pos++;

				if(pos < str.size() && DIGITO.find(str[pos]) != DIGITO.end())
				{
					lexema = lexema + str[pos];
					pos++;					
				}
				//else possivel ERROR

				while(pos < str.size() && DIGITO.find(str[pos]) != DIGITO.end())
				{
					lexema = lexema + str[pos];
					pos++; 
				}
			}

			if(str[pos] == 'E' || str[pos] == 'e')
			{
				lexema = lexema + str[pos];
				pos++;

				if(pos < str.size() && str[pos] == '+' || str[pos] == '-')
				{
					lexema = lexema + str[pos];
					pos++;
				}

				if(pos < str.size() && DIGITO.find(str[pos]) != DIGITO.end())
				{
					lexema = lexema + str[pos];
					pos++;
				}
				//else{} POSSIVEL ERROR

				while(pos < str.size() && DIGITO.find(str[pos]) != DIGITO.end())
				{
					lexema = lexema + str[pos];
					pos++; 
				}

				pos--;

			}
			else
			{
				pos--;
			}

		}
		else
		{
			pos--;
		}

		if(!achou_real)
			imprimir_token("NUM", lexema, "inteiro");
		else
			imprimir_token("NUM", lexema, "real");

	}


	return pos;
}

//Lit com aspas duplas
int token_Lit_com_aspas_duplas(string str, int pos, int linha)
{
	string lexema = "";
	bool deu_certo = true;

	if(str[pos] == '"')
	{
		lexema = lexema + str[pos];
		pos++;
		while(pos < str.size() && str[pos] != '"')
		{
			lexema = lexema + str[pos];	
			pos++;
		}
		
		if(pos < str.size() && str[pos] == '"')
		{
			lexema = lexema + str[pos];		
		}

	}

	if(deu_certo)
	{
		imprimir_token("Lit", lexema, "literal");
	}

	return pos;	
}

//Lit com aspas simples
int token_Lit_com_aspas_simples(string str, int pos, int linha)
{
	string lexema = "";
	bool deu_certo = true;

	if(str[pos] == '\'')
	{
		lexema = lexema + str[pos];
		pos++;
		
		if(pos < str.size() && str[pos] != '\'')
		{
			lexema = lexema + str[pos];	
			pos++;
		}
		
		if(pos < str.size() && str[pos] == '\'')
		{
			lexema = lexema + str[pos];		
		}
	}

	if(deu_certo)
	{
		imprimir_token("Lit", lexema, "literal");
	}

	return pos;	
}

//ID
int token_id(string str, int pos, int linha)
{
	string lexema = "";
	bool deu_certo = true;

	if(LETRA.find(str[pos]) != LETRA.end())
	{
		lexema = lexema + str[pos];
		pos++;	

		while(pos < str.size() && (LETRA.find(str[pos]) != LETRA.end() || DIGITO.find(str[pos]) != DIGITO.end() || str[pos] == '_'))
		{
			lexema = lexema + str[pos];	
			pos++;
		}
		pos--;

		bool achou = false;
		for(int i = 0; i < PALAVRAS_RESERVADAS.size(); i++)
		{
			IDENTIFICADOR id = PALAVRAS_RESERVADAS[i];
			if(id.lexema == lexema)
			{
				imprimir_token(id.classe, id.lexema, id.tipo);
				achou = true;
				//cout << "achou krl\n";
			}
		}
		if(!achou)
		{
			//cout << "NAO\n";
			PALAVRAS_RESERVADAS.push_back(identificador("id", lexema, "Nulo"));
			imprimir_token("id", lexema, "Nulo");
		}

	}


	return pos;
}

//Comentario
int token_Comentario(string str, int pos, int linha)
{
	string lexema = "";
	bool deu_certo = true;

	if(str[pos] == '{')
	{
		lexema = lexema + str[pos];
		pos++;
		while(pos < str.size() && str[pos] != '}')
		{
			lexema = lexema + str[pos];	
			pos++;
		}
		
		if(pos < str.size() && str[pos] == '}')
		{
			lexema = lexema + str[pos];		
		}

	}

	if(deu_certo)
	{
		imprimir_token("Comentário", lexema, "Nulo");
	}

	return pos;
}

//EOF
//ERRADOOOOOOOOOO
//exemplo no doc que ela fez
/*int token_EOF(string str, int pos)
{
	string lexema = "";

	if(str[pos] == '$')
	{
		lexema = lexema + str[pos];
	}

	imprimir_token("Fim de arquivo(EOF)", lexema, "Nulo");

	return pos;
}*/

//RCB ou OPR
int token_RCB_ou_OPR(string str, int pos, int linha)
{
	string lexema = "";

	if(str[pos] == '>')
	{
		lexema = lexema + str[pos];
		pos++;

		if(pos < str.size())
		{
			if(str[pos] == '=')
			{
				lexema = lexema + str[pos];
			}
			else
			{
				pos--;
			}
		}
		else
		{
			pos--;
		}

		imprimir_token("OPR", lexema, "Nulo");
		
	}
	else if(str[pos] == '=')
	{
		lexema = lexema + str[pos];
		imprimir_token("OPR", lexema, "Nulo");

	}
	else if(str[pos] == '<')
	{
		lexema = lexema + str[pos];
		pos++;

		if(pos < str.size())
		{
			if(str[pos] == '-')
			{
				lexema = lexema + str[pos];
				imprimir_token("RCB", lexema, "Nulo");
			}
			else if(str[pos] == '=' || str[pos] == '>')
			{
				lexema = lexema + str[pos];
				imprimir_token("OPR", lexema, "Nulo");
			}
			else
			{
				pos--;
				imprimir_token("OPR", lexema, "Nulo");
			}
		}
		else
		{
			pos--;
			imprimir_token("OPR", lexema, "Nulo");
		}
	}

	return pos;
}

//OPM
int token_OPM(string str, int pos)
{	
	string lexema = "";

	if(str[pos] == '+' || str[pos] == '-' || str[pos] == '*' || str[pos] == '/')
	{
		lexema = lexema + str[pos];
	}

	imprimir_token("OPM", lexema, "Nulo");

	return pos;	
}

//AB_P
int token_AB_P(string str, int pos)
{
	string lexema = "";

	if(str[pos] == '(')
	{
		lexema = lexema + str[pos];
	}

	imprimir_token("AB_P", lexema, "Nulo");

	return pos;
}

//FC_P
int token_FC_P(string str, int pos)
{
	string lexema = "";

	if(str[pos] == ')')
	{
		lexema = lexema + str[pos];
	}
	
	imprimir_token("FC_P", lexema, "Nulo");

	return pos;
}

//PT_V
int token_PT_V(string str, int pos)
{
	string lexema = "";

	if(str[pos] == ';')
	{
		lexema = lexema + str[pos];
	}
	
	imprimir_token("PT_V", lexema, "Nulo");

	return pos;
}

//VIR
int token_VIR(string str, int pos)
{
	string lexema = "";

	if(str[pos] == ',')
	{
		lexema = lexema + str[pos];
	}
	
	imprimir_token("VIR", lexema, "Nulo");

	return pos;
}

int main()
{
	//SETUP
	//PALAVRAS_RESERVADAS.insert({{"inicio", {"inicio", "Nulo"}}, {"ola", {"inicio", "Nulo"}}});
	PALAVRAS_RESERVADAS.push_back(identificador("inicio", "inicio", "Nulo"));
	PALAVRAS_RESERVADAS.push_back(identificador("varinicio", "varinicio", "Nulo"));
	PALAVRAS_RESERVADAS.push_back(identificador("varfim", "varfim", "Nulo"));
	PALAVRAS_RESERVADAS.push_back(identificador("escreva", "escreva", "Nulo"));
	PALAVRAS_RESERVADAS.push_back(identificador("leia", "leia", "Nulo"));
	PALAVRAS_RESERVADAS.push_back(identificador("se", "se", "Nulo"));
	PALAVRAS_RESERVADAS.push_back(identificador("entao", "entao", "Nulo"));
	PALAVRAS_RESERVADAS.push_back(identificador("fimse", "fimse", "Nulo"));
	PALAVRAS_RESERVADAS.push_back(identificador("repita", "repita", "Nulo"));
	PALAVRAS_RESERVADAS.push_back(identificador("fimrepita", "fimrepita", "Nulo"));
	PALAVRAS_RESERVADAS.push_back(identificador("fim", "fim", "Nulo"));
	PALAVRAS_RESERVADAS.push_back(identificador("inteiro", "inteiro", "inteiro"));
	PALAVRAS_RESERVADAS.push_back(identificador("literal", "literal", "literal"));
	PALAVRAS_RESERVADAS.push_back(identificador("real", "real", "real"));

	//ERROS
	ERROR["ERROR1"] = "Caracter invalido na linguagem";
	ERROR["ERROR2"] = "Numero invalido";
	ERROR["ERROR3"] = "Faltando uma aspas duplas";
	ERROR["ERROR4"] = "Faltando uma aspas simples";
	ERROR["ERROR5"] = "Faltando chaves }";
	ERROR["ERROR6"] = "Aspas simples não suporta mais de um caracter";
	ERROR["ERROR7"] = "A aspas simples nao pode ser vazia";

	//DIGITO
	DIGITO.insert({'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'});

	//LETRA
	LETRA.insert({'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'});

	//lendo arquivo fonte
	ifstream file("CodigoFonte.txt");
    string linha; 
    
    int n_linha = 1;
	while(getline(file, linha))
	{
		cout << "linha: " << linha << endl;
		for(int pos = 0; pos < linha.size(); pos++)
		{
			//eliminando espaco
			while(linha[pos] == ' ')
				pos++;

			//cout << "letra: " << linha[pos] << endl;
			if(DIGITO.find(linha[pos]) != DIGITO.end()) //Num
			{
				pos = token_Num(linha, pos, n_linha);
			}
			else if(linha[pos] == '"' || linha[pos] == '\'') //Literal
			{
				if(linha[pos] == '"') //com aspas duplas
				{
					pos = token_Lit_com_aspas_duplas(linha, pos, n_linha);
				}
				else if(linha[pos] == '\'') //com aspas simples
				{
					pos = token_Lit_com_aspas_simples(linha, pos, n_linha);	
				}
			}
			else if(LETRA.find(linha[pos]) != LETRA.end()) //id
			{
				pos = token_id(linha, pos, n_linha);
			}
			else if(linha[pos] == '{') //Comentario
			{
				pos = token_Comentario(linha, pos, n_linha);
			}
			else if(linha[pos] == '$') //Fim de arquivo(EOF)
			{
				cout << "ARRUMAR ISSO AQUI" << endl;
				//pos = token_EOF(linha, pos);
			}
			else if(linha[pos] == '<' || linha[pos] == '>' || linha[pos] == '=') //RCB ou OPR
			{
				pos = token_RCB_ou_OPR(linha, pos, n_linha);
			}
			else if(linha[pos] == '+' || linha[pos] == '-' || linha[pos] == '*' || linha[pos] == '/') //OPM
			{
				pos = token_OPM(linha, pos);
			}
			else if(linha[pos] == '(') //AB_P
			{
				pos = token_AB_P(linha, pos);
			}
			else if(linha[pos] == ')') //FC_P
			{
				pos = token_FC_P(linha, pos);
			}
			else if(linha[pos] == ';') //PT_V
			{
				pos = token_PT_V(linha, pos);
			}
			else if(linha[pos] == ',') //VIR
			{
				pos = token_VIR(linha, pos);
			}
			//else possivelerror

		}
		
		n_linha++;	
	}





	return 0;
}






//utils

/*if(DIGITO.find('1') != DIGITO.end())
	{
		cout << "ok";
	}
	if(DIGITO.find('4') != DIGITO.end())
	{
		cout << "ok2";
	}*/

//cout << "ola: " << PALAVRAS_RESERVADAS["inicio"].first << endl;
