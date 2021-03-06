#include <bits/stdc++.h>
#include <cstdlib>
#include <unistd.h>

using namespace std;

struct token
{
	string classe;
	string lexema;
	string tipo;
};

typedef struct token TOKEN;
typedef pair<int, string> pis;
typedef pair<string, string> pss;

//variaveis globais
vector<TOKEN> PALAVRAS_RESERVADAS;
vector<string> TERMINAIS;
vector<string> NAO_TERMINAIS;
vector<string> linhas;

unordered_map <string, string> ERROS;

unordered_set <char> DIGITO;
unordered_set <char> LETRA;
unordered_set <char> ALFABETO;

map <pis, string> GOTO;
map <pis, string> ACTION;
map <int, pss> REGRAS_GRAMATICAS;

int pos, n_linha = 1;

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

//imprimir mensagem
void imprimir_mensagem(TOKEN new_token, int n_linha)
{
	if(new_token.classe != "Comentario")
		imprimir_token(new_token.classe, new_token.lexema, new_token.tipo);
	
	if(ERROS.find(new_token.classe) != ERROS.end())
		imprimir_erro(new_token.classe, n_linha, pos + 1);
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
			return {"num", lexema, "inteiro"};
		else
			return {"num", lexema, "real"};
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
		return {"lit", lexema, "literal"};
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

	return {"lit", lexema, "literal"};
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
		return {"Comentario", lexema, "Nulo"};
	else
		return {"ERRO5", lexema, "Nulo"};
}

//EOF
TOKEN token_EOF(string str)
{
	string lexema = "";

	if(str[pos] == '$')
		lexema = lexema + "EOF";

	return {"$", lexema, "Nulo"};
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

		return {"opr", lexema, "Nulo"};
	}
	else if(str[pos] == '=')
	{
		lexema = lexema + str[pos];
		return {"opr", lexema, "Nulo"};
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
				return {"rcb", lexema, "Nulo"};
			}
			else if(str[pos] == '=' || str[pos] == '>')
			{
				lexema = lexema + str[pos];
				return {"opr", lexema, "Nulo"};
			}
			else
			{
				pos--;
				return {"opr", lexema, "Nulo"};
			}
		}
		else
		{
			pos--;
			return {"opr", lexema, "Nulo"};
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

	return {"opm", lexema, "Nulo"};
}

//AB_P
TOKEN token_AB_P(string str)
{
	string lexema = "";

	if(str[pos] == '(')
		lexema = lexema + str[pos];

	return {"ab_p", lexema, "Nulo"};
}

//FC_P
TOKEN token_FC_P(string str)
{
	string lexema = "";

	if(str[pos] == ')')
		lexema = lexema + str[pos];
	
	return {"fc_p", lexema, "Nulo"};
}

//PT_V
TOKEN token_PT_V(string str)
{
	string lexema = "";

	if(str[pos] == ';')
		lexema = lexema + str[pos];
	
	return {"pt_v", lexema, "Nulo"};
}

//VIR
TOKEN token_VIR(string str)
{
	string lexema = "";

	if(str[pos] == ',')
		lexema = lexema + str[pos];
	
	return {"vir", lexema, "Nulo"};
}

//scanner ?? a funcao onde retornar?? o token
TOKEN scanner(string linha)
{
	//eliminando espaco
	while(linha[pos] == ' ')
		pos++;

	if(pos >= linha.size()) //linha sem token
	{
		return {"", "", "Nulo"};
	}
	else if(DIGITO.find(linha[pos]) != DIGITO.end()) //Num
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

//pegar um token
TOKEN get_token(string linha)
{
	bool new_token_vazio = true;
	TOKEN new_token;
	int classe_size;

	while(new_token_vazio) //enquanto token esta vazio
	{
		if(pos >= linha.size()) //muda de linha caso nao tenha mais nada na linha
		{
			pos = 0;
			n_linha++;
			linha = linhas[n_linha - 1];
		}
		
		new_token = scanner(linha);
		pos++;
		classe_size = new_token.classe.size();

		if(new_token.classe != "" && ERROS.find(new_token.classe) == ERROS.end() && new_token.classe.substr(0, classe_size) != "Comentario")
			new_token_vazio = false;

		if(ERROS.find(new_token.classe) != ERROS.end())
		{
			cout << "ERRO LEXICO -- ";
			imprimir_mensagem(new_token, n_linha);
		}
	}

	//imprimir_mensagem(new_token, n_linha);

	return new_token;
}

//pega cada linha do csv, cada palavra da linha e adiciona no conjunto de palavras
vector<vector<string>> read_table(string file)
{
	ifstream file_csv(file);
    vector<vector<string>> tabela;
    string linha_csv;
    
	while(getline(file_csv, linha_csv))
	{
		vector<string> palavras_linha_csv;
	    stringstream ss(linha_csv);
	 
	    while (ss.good()) 
	    {
	        string substr;
	        getline(ss, substr, ',');
	        palavras_linha_csv.push_back(substr);
	    }
	 
	 	tabela.push_back(palavras_linha_csv);
	}

	return tabela;
}

//constroi a tabela ACTION e GOTO
map <pis, string> build_table_SLR(string file)
{	
	vector<vector<string>> tabela = read_table(file);
	vector<string> terminais(tabela[0]); //terminais ou nao terminais
	map <pis, string> SLR;

	for (int i = 1; i < tabela.size(); ++i)
	{
		for (int j = 0; j < tabela[i].size(); ++j)
		{
			string valor = tabela[i][j];

			if(valor != "")
			{
				int estado = i - 1;
				string terminal = terminais[j];
				SLR[{estado, terminal}] = valor;
			}
		}
	}

	return SLR;	
}

//tamanho de beta
int tamanho_de_beta(string beta)
{
	int i = 0, sum = 0;

	while(i < beta.size())
	{
		while(i < beta.size() && beta[i] == ' ')
			i++;

		while(i < beta.size() && beta[i] != ' ')
			i++;		

		sum++;
	}

	return sum;
}

//pega linha atual
string get_linha()
{
	return linhas[n_linha - 1];
}

void parser()
{
	stack<int> pilha; //pilha
	string a, beta, A, acao, linha, lex; //a(classe), A -> beta(regra gramatical), acao(tabela action), lex(lexema), linha(linha atual)
	TOKEN new_token; //token
	int s, t; //estados
	
	n_linha = 1;
	pos = 0;
	
	linha = get_linha();
	new_token = get_token(linha);

	pilha.push(0); //comeca com o estado 0

	a = new_token.classe; //pega a classe

	while(true)
	{
		s = pilha.top(); //pega o primeiro estado do topo da pilha
		acao = ACTION[{s, a}]; //acessa a tabela ACTION e pega a acao

		if(acao[0] == 's') //shift
		{
			t = stoi(acao.substr(1, acao.size() - 1), nullptr, 10); //pega o estado da acao shift. e.g s40, pega o 40
			
			pilha.push(t); //coloca o estado no topo da pilha
			
			linha = get_linha(); //pega a linha atual
			new_token = get_token(linha); //pega o token

			a = new_token.classe;
		}
		else if(acao[0] == 'r') //reduce
		{
			int estado = stoi(acao.substr(1, acao.size() - 1), nullptr, 10); //pega o estado da acao reduce. e.g r7, pega o 7
			pss gramatica = REGRAS_GRAMATICAS[estado]; //pega a REGRA GRAMATICAL do estado
			
			//gramatica: A -> beta
			A = gramatica.first; //lado esquerdo da producao
			beta = gramatica.second; //lado direito da producao
			int beta_size = tamanho_de_beta(beta); //pega o tamanho de beta

			//deleta uma quantidade de beta_size valores na pilha
			for(int i = 0; i < beta_size; i++)
				pilha.pop();

			t = pilha.top(); //pega o topo da pilha
			acao = GOTO[{t, A}]; // acessa a tabela GOTO e pega o estado
			
			pilha.push(stoi(acao, nullptr, 10)); //coloca o estado dentro da pilha

			cout << A << "->" << beta << endl; //imprime a REGRA GRAMATICAL: A -> beta
		}
		else if(acao == "ACEITO")
		{
			//pega a REGRA GRAMATICAL do estado
			pss gramatica = REGRAS_GRAMATICAS[s];
			A = gramatica.first;
			beta = gramatica.second;

			cout << A << "->" << beta << endl; //imprime a REGRA GRAMATICAL: A -> beta
 			break;
		}
		else //TRATAMENTO DE ERRO
		{	
			string possivel_erro;

			//possivel erro
			for(int i = 0; TERMINAIS.size(); i++)
			{
				possivel_erro = TERMINAIS[i];
				if(ACTION[{s, possivel_erro}] != "")
				{
					break;
				}
			}

			cout << "ERRO SINTATICO - Faltando um token " << possivel_erro << " na linha " << n_linha << ", coluna " << pos << endl; // imprimindo o erro
			bool goto_valido = false; 
			string novo_estado;
			TOKEN token;

			//procura por um GOTO valido na pilha
			while(!pilha.empty() && !goto_valido)
			{
				s = pilha.top(); // pega o estado do topo da pilha
				
				//percorre todos os nao terminais em busca de um goto valido
				for (int i = 0; i < NAO_TERMINAIS.size(); ++i)
				{
					if(GOTO[{s, NAO_TERMINAIS[i]}] != "")
					{
						goto_valido = true;
						novo_estado = GOTO[{s, NAO_TERMINAIS[i]}]; //pega o novo estado
						break;
					}
				}

				if(goto_valido)
					pilha.push(stoi(novo_estado, nullptr, 10)); //coloca na pilha o novo estado
				else
					pilha.pop(); //deleta da pilha, nao achou um goto valido

				if(pilha.empty()) //se a pilha for vazia imprime uma mensagem
					cout << "PILHA VAZIA\n";
			}

			s = pilha.top(); //pega o topo da pilha

			//procura por um ACTION valido pegando os tokens
			while(true)
			{	
				if(ACTION[{s, a}] != "")
					break;	

				if(a == "$") // caso o ultimo token seja $ e nao tem ACTION entre o novo estado e $ 
				{
					cout << "NAO ACEITO, FINALIZOU O PARSER\n";
					return;
				}

				linha = get_linha(); //pega a linha atual
				token = get_token(linha); //pega um token
				a = token.classe; //pega a classe
				
			}
		}
	}

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

	//REGRAS GRAMATICAS
	REGRAS_GRAMATICAS[1] = {"P'", "P"};
	REGRAS_GRAMATICAS[2] = {"P", "inicio V A"};
	REGRAS_GRAMATICAS[3] = {"V", "varinicio LV"};
	REGRAS_GRAMATICAS[4] = {"LV", "D LV"};
	REGRAS_GRAMATICAS[5] = {"LV", "varfim pt_v"};
	REGRAS_GRAMATICAS[6] = {"D", "TIPO L pt_v"};
	REGRAS_GRAMATICAS[7] = {"L", "id vir L"};
	REGRAS_GRAMATICAS[8] = {"L", "id"};
	REGRAS_GRAMATICAS[9] = {"TIPO", "inteiro"};
	REGRAS_GRAMATICAS[10] = {"TIPO", "real"};
	REGRAS_GRAMATICAS[11] = {"TIPO", "literal"};
	REGRAS_GRAMATICAS[12] = {"A", "ES A"};
	REGRAS_GRAMATICAS[13] = {"ES", "leia id pt_v"};
	REGRAS_GRAMATICAS[14] = {"ES", "escreva ARG pt_v"};
	REGRAS_GRAMATICAS[15] = {"ARG", "lit"};
	REGRAS_GRAMATICAS[16] = {"ARG", "num"};
	REGRAS_GRAMATICAS[17] = {"ARG", "id"};
	REGRAS_GRAMATICAS[18] = {"A", "CMD A"};
	REGRAS_GRAMATICAS[19] = {"CMD", "id rcb LD pt_v"};
	REGRAS_GRAMATICAS[20] = {"LD", "OPRD opm OPRD"};
	REGRAS_GRAMATICAS[21] = {"LD", "OPRD"};
	REGRAS_GRAMATICAS[22] = {"OPRD", "id"};
	REGRAS_GRAMATICAS[23] = {"OPRD", "num"};
	REGRAS_GRAMATICAS[24] = {"A", "COND A"};
	REGRAS_GRAMATICAS[25] = {"COND", "CAB CP"};
	REGRAS_GRAMATICAS[26] = {"CAB", "se ab_p EXP_R fc_p ent??o"};
	REGRAS_GRAMATICAS[27] = {"EXP_R", "OPRD opr OPRD"};
	REGRAS_GRAMATICAS[28] = {"CP", "ES CP"};
	REGRAS_GRAMATICAS[29] = {"CP", "CMD CP"};
	REGRAS_GRAMATICAS[30] = {"CP", "COND CP"};
	REGRAS_GRAMATICAS[31] = {"CP", "fimse"};
	REGRAS_GRAMATICAS[32] = {"A", "R A"};
	REGRAS_GRAMATICAS[33] = {"R", "repita ab_p EXP_R fc_p CP_R"};
	REGRAS_GRAMATICAS[34] = {"CP_R", "ES CP_R"};
	REGRAS_GRAMATICAS[35] = {"CP_R", "CMD CP_R"};
	REGRAS_GRAMATICAS[36] = {"CP_R", "COND CP_R"};
	REGRAS_GRAMATICAS[37] = {"CP_R", "fimrepita"};
	REGRAS_GRAMATICAS[38] = {"A", "fim"};

	//TERMINAIS
	TERMINAIS.push_back("inicio");
	TERMINAIS.push_back("varinicio");
	TERMINAIS.push_back("varfim");
	TERMINAIS.push_back("pt_v");
	TERMINAIS.push_back("id");
	TERMINAIS.push_back("vir");
	TERMINAIS.push_back("inteiro");
	TERMINAIS.push_back("real");
	TERMINAIS.push_back("literal");
	TERMINAIS.push_back("leia");
	TERMINAIS.push_back("escreva");
	TERMINAIS.push_back("lit");
	TERMINAIS.push_back("num");
	TERMINAIS.push_back("rcb");
	TERMINAIS.push_back("opm");
	TERMINAIS.push_back("se");
	TERMINAIS.push_back("fimse");
	TERMINAIS.push_back("ab_p");
	TERMINAIS.push_back("fc_p");
	TERMINAIS.push_back("entao");
	TERMINAIS.push_back("opr");
	TERMINAIS.push_back("repita");
	TERMINAIS.push_back("fimrepita");
	TERMINAIS.push_back("fim");
	TERMINAIS.push_back("$");

	//NAO_TERMINAIS
	NAO_TERMINAIS.push_back("P");
	NAO_TERMINAIS.push_back("V");
	NAO_TERMINAIS.push_back("LV");
	NAO_TERMINAIS.push_back("D");
	NAO_TERMINAIS.push_back("L");
	NAO_TERMINAIS.push_back("TIPO");
	NAO_TERMINAIS.push_back("A");
	NAO_TERMINAIS.push_back("ES");
	NAO_TERMINAIS.push_back("ARG");
	NAO_TERMINAIS.push_back("CMD");
	NAO_TERMINAIS.push_back("LD");
	NAO_TERMINAIS.push_back("OPRD");
	NAO_TERMINAIS.push_back("COND");
	NAO_TERMINAIS.push_back("CAB");
	NAO_TERMINAIS.push_back("EXP_R");
	NAO_TERMINAIS.push_back("CP");
	NAO_TERMINAIS.push_back("R");
	NAO_TERMINAIS.push_back("CP_R");

	//tabela GOTO
	GOTO = build_table_SLR("./tabela/tabela_nao_terminais.csv");

	//tabela ACTION
	ACTION = build_table_SLR("./tabela/tabela_terminais.csv");

	//lendo arquivo fonte
	ifstream file("tudo_certo_teste_professora.txt");
    
    string linha;
    
    //lendo o arquivo linha por linha
    while(getline(file, linha))
    	linhas.push_back(linha);

    //colocando um final de arquivo no final
    linha = "$";
    linhas.push_back(linha);

	parser();

	file.close();
	
	return 0;
}
