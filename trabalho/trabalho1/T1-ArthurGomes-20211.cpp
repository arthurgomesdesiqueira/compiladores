#include <bits/stdc++.h>

using namespace std;

struct token
{
	string classe;
	string lexema;
	string tipo;
};

typedef struct token TOKEN;

//variaveis globais
vector<TOKEN> PALAVRAS_RESERVADAS;
unordered_map <string, string> ERROS;
unordered_set <char> DIGITO;
unordered_set <char> LETRA;
unordered_set <char> ALFABETO;

int pos;

//Imprimir o token
void imprimir_token(string token, string lexema, string tipo)
{
	cout << "Classe: " << token << ", Lexema: " << lexema << ", Tipo: " << tipo << endl;
}

//Imprimir o erro
void imprimir_erro(string tipo_erro, int linha, int coluna)
{
	cout << tipo_erro << " - " << ERROS[tipo_erro] << " , linha " << linha << ", coluna " << coluna << endl;
}

//NUM
TOKEN token_Num(string str)
{
	string lexema = "";

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
				else
				{	
					pos--;
					return {"ERRO2", lexema, "Nulo"};					
				}

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
				else
				{	
					pos--;
					return {"ERRO2", lexema, "Nulo"};
				}

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
			return {"NUM", lexema, "inteiro"};
		else
			return {"NUM", lexema, "real"};
	}

	return {};
}

//Lit com aspas duplas
TOKEN token_Lit_com_aspas_duplas(string str)
{
	string lexema = "";
	bool deu_certo = false;

	if(str[pos] == '"')
	{
		lexema = lexema + str[pos];
		pos++;
		
		while(pos < str.size() && str[pos] != '"' && ALFABETO.find(str[pos]) != ALFABETO.end())
		{
			lexema = lexema + str[pos];	
			pos++;
		}

		if(pos < str.size() && str[pos] == '"')
		{
			lexema = lexema + str[pos];	
			pos++;
			deu_certo = true;	
		}
		
	}

	pos--;

	if(deu_certo)
		return {"Lit", lexema, "literal"};
	else
		return {"ERRO3", lexema, "Nulo"};	
}

//Lit com aspas simples
TOKEN token_Lit_com_aspas_simples(string str)
{
	string lexema = "";

	if(str[pos] == '\'')
	{
		lexema = lexema + str[pos];
		pos++;
		
		if(pos < str.size() && str[pos] != '\'' && ALFABETO.find(str[pos]) != ALFABETO.end())
		{
			lexema = lexema + str[pos];	
			pos++;
		}
		else if(pos == str.size() || ALFABETO.find(str[pos]) == ALFABETO.end())
		{
			pos--;
			return {"ERRO4", lexema, "Nulo"};
		}
		else if(pos < str.size() && str[pos] == '\'')
		{
			lexema = lexema + str[pos];
			return {"ERRO6", lexema, "Nulo"};	
		}

		if(pos < str.size() && str[pos] == '\'')
		{
			lexema = lexema + str[pos];
		}
		else if(pos == str.size() || str[pos] != '\'')
		{
			pos--;
			return {"ERRO4", lexema, "Nulo"};			
		}
		
	}

	return {"Lit", lexema, "literal"};
}

//ID
TOKEN token_id(string str)
{
	string lexema = "";

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

		for(int i = 0; i < PALAVRAS_RESERVADAS.size(); i++)
		{
			TOKEN id = PALAVRAS_RESERVADAS[i];
			
			if(id.lexema == lexema) //achou dentro da variavel PALAVFRAS_RESERVADAS
			{
				return {id.classe, id.lexema, id.tipo};
			}
		}

		PALAVRAS_RESERVADAS.push_back({"id", lexema, "Nulo"}); //adiciona o token dentro da variavel PALAVFRAS_RESERVADAS
		return {"id", lexema, "Nulo"};
	}

	return {};
}

//Comentario
TOKEN token_Comentario(string str)
{
	string lexema = "";
	bool deu_certo = false;

	if(str[pos] == '{')
	{
		lexema = lexema + str[pos];
		pos++;
		
		while(pos < str.size() && str[pos] != '}' && ALFABETO.find(str[pos]) != ALFABETO.end())
		{
			lexema = lexema + str[pos];	
			pos++;
		}
		
		if(pos < str.size() && str[pos] == '}')
		{
			lexema = lexema + str[pos];		
			pos++;
			deu_certo = true;
		}
	}

	pos--;

	if(deu_certo)
		return {"Comentário", lexema, "Nulo"};
	else
		return {"ERRO5", lexema, "Nulo"};
}

//EOF
TOKEN token_EOF(string str)
{
	string lexema = "";

	if(str[pos] == '$')
		lexema = lexema + "EOF";

	return {"EOF", lexema, "Nulo"};
}

//RCB ou OPR
TOKEN token_RCB_ou_OPR(string str)
{
	string lexema = "";

	if(str[pos] == '>')
	{
		lexema = lexema + str[pos];
		pos++;

		if(pos < str.size())
		{
			if(str[pos] == '=')
				lexema = lexema + str[pos];
			else
				pos--;
		}
		else
		{
			pos--;
		}

		return {"OPR", lexema, "Nulo"};
	}
	else if(str[pos] == '=')
	{
		lexema = lexema + str[pos];
		return {"OPR", lexema, "Nulo"};
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
				return {"RCB", lexema, "Nulo"};
			}
			else if(str[pos] == '=' || str[pos] == '>')
			{
				lexema = lexema + str[pos];
				return {"OPR", lexema, "Nulo"};
			}
			else
			{
				pos--;
				return {"OPR", lexema, "Nulo"};
			}
		}
		else
		{
			pos--;
			return {"OPR", lexema, "Nulo"};
		}
	}

	return {};
}

//OPM
TOKEN token_OPM(string str)
{	
	string lexema = "";

	if(str[pos] == '+' || str[pos] == '-' || str[pos] == '*' || str[pos] == '/')
		lexema = lexema + str[pos];

	return {"OPM", lexema, "Nulo"};
}

//AB_P
TOKEN token_AB_P(string str)
{
	string lexema = "";

	if(str[pos] == '(')
		lexema = lexema + str[pos];

	return {"AB_P", lexema, "Nulo"};
}

//FC_P
TOKEN token_FC_P(string str)
{
	string lexema = "";

	if(str[pos] == ')')
		lexema = lexema + str[pos];
	
	return {"FC_P", lexema, "Nulo"};
}

//PT_V
TOKEN token_PT_V(string str)
{
	string lexema = "";

	if(str[pos] == ';')
		lexema = lexema + str[pos];
	
	return {"PT_V", lexema, "Nulo"};
}

//VIR
TOKEN token_VIR(string str)
{
	string lexema = "";

	if(str[pos] == ',')
		lexema = lexema + str[pos];
	
	return {"VIR", lexema, "Nulo"};
}

//scanner é a funcao onde retornará o token
TOKEN scanner(string linha)
{
	//eliminando espaco
	while(linha[pos] == ' ')
		pos++;

	if(DIGITO.find(linha[pos]) != DIGITO.end()) //Num
	{
		return token_Num(linha);
	}
	else if(linha[pos] == '"' || linha[pos] == '\'') //Literal
	{
		if(linha[pos] == '"') //com aspas duplas
		{
			return token_Lit_com_aspas_duplas(linha);
		}
		else if(linha[pos] == '\'') //com aspas simples
		{
			return token_Lit_com_aspas_simples(linha);	
		}
	}
	else if(LETRA.find(linha[pos]) != LETRA.end()) //id
	{
		return token_id(linha);
	}
	else if(linha[pos] == '{') //Comentario
	{
		return token_Comentario(linha);
	}
	else if(linha[pos] == '$') //Fim de arquivo(EOF)
	{
		return token_EOF(linha);
	}
	else if(linha[pos] == '<' || linha[pos] == '>' || linha[pos] == '=') //RCB ou OPR
	{
		return token_RCB_ou_OPR(linha);
	}
	else if(linha[pos] == '+' || linha[pos] == '-' || linha[pos] == '*' || linha[pos] == '/') //OPM
	{
		return token_OPM(linha);
	}
	else if(linha[pos] == '(') //AB_P
	{
		return token_AB_P(linha);
	}
	else if(linha[pos] == ')') //FC_P
	{
		return token_FC_P(linha);
	}
	else if(linha[pos] == ';') //PT_V
	{
		return token_PT_V(linha);
	}
	else if(linha[pos] == ',') //VIR
	{
		return token_VIR(linha);
	}
	
	string lexema = "";
	lexema = lexema + linha[pos];
	
	return {"ERRO1", lexema, "Nulo"};


}

int main()
{	
	//PALAVRAS RESERVADAS
	PALAVRAS_RESERVADAS.push_back({"inicio", "inicio", "Nulo"});
	PALAVRAS_RESERVADAS.push_back({"varinicio", "varinicio", "Nulo"});
	PALAVRAS_RESERVADAS.push_back({"varfim", "varfim", "Nulo"});
	PALAVRAS_RESERVADAS.push_back({"escreva", "escreva", "Nulo"});
	PALAVRAS_RESERVADAS.push_back({"leia", "leia", "Nulo"});
	PALAVRAS_RESERVADAS.push_back({"se", "se", "Nulo"});
	PALAVRAS_RESERVADAS.push_back({"entao", "entao", "Nulo"});
	PALAVRAS_RESERVADAS.push_back({"fimse", "fimse", "Nulo"});
	PALAVRAS_RESERVADAS.push_back({"repita", "repita", "Nulo"});
	PALAVRAS_RESERVADAS.push_back({"fimrepita", "fimrepita", "Nulo"});
	PALAVRAS_RESERVADAS.push_back({"fim", "fim", "Nulo"});
	PALAVRAS_RESERVADAS.push_back({"inteiro", "inteiro", "inteiro"});
	PALAVRAS_RESERVADAS.push_back({"literal", "literal", "literal"});
	PALAVRAS_RESERVADAS.push_back({"real", "real", "real"});

	//ERROS
	ERROS["ERRO1"] = "Caracter invalido na linguagem";
	ERROS["ERRO2"] = "Numero invalido";
	ERROS["ERRO3"] = "Literal incompleto - Faltando uma aspas duplas";
	ERROS["ERRO4"] = "Literal incompleto - Faltando uma aspas simples";
	ERROS["ERRO5"] = "Comentario incompleto - Faltando chaves }";
	ERROS["ERRO6"] = "Literal incompleto - A aspas simples nao pode ser vazia";
	
	//DIGITO
	DIGITO.insert({'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'});

	//LETRA
	LETRA.insert({'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'});

	//ALFABETO
	ALFABETO.insert(DIGITO.begin(), DIGITO.end());
	ALFABETO.insert(LETRA.begin(), LETRA.end());
	ALFABETO.insert({';', '(', ')', '{', '}', '*', '/', '-', '+', '"', '\'', '_', '<', '>', '=', ',', ' '});

	//lendo arquivo fonte
	ifstream file("CodigoFonte.txt");
    
    string linha;
    TOKEN new_token;
    int n_linha = 1;

	while(getline(file, linha))
	{
		for(pos = 0; pos < linha.size(); pos++)
		{
			new_token = scanner(linha);

			if(new_token.classe != "Comentário")
				imprimir_token(new_token.classe, new_token.lexema, new_token.tipo);

			if(ERROS.find(new_token.classe) != ERROS.end())
				imprimir_erro(new_token.classe, n_linha, pos + 1);
		}

		n_linha++;
	}

	//adicionar EOF no final
	pos = 0;
	linha = "$";
	new_token = scanner(linha);
	imprimir_token(new_token.classe, new_token.lexema, new_token.tipo);

	return 0;
}

