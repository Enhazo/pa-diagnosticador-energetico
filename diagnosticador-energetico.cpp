/*
Nome do programa......: diagnosticador energetico 2000
Descricao do Programa.: monitoramento de energia de 4 setores por 5 dias seguidos,
                       usando matriz 4 por 5, com confirmação por setor.
Autor.................: Enhazo
Data de criação.......: 28/10/2026 as 11:23
Data de Atualização...: 02/11/2026 as 20:24
*/

#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <windows.h>
#include <locale.h>

using namespace std;

int main()
{
    // arruma o console pra portugues e utf-8
    if (__GNUC__ <= 9) {
        setlocale(LC_ALL, "Portuguese_Brazil");    
    } else {
        SetConsoleOutputCP(CP_UTF8);    
    }

    cout << fixed << setprecision(2);

    // Substituindo o while por um for para controle de reexecução
    // O primeiro e o terceiro campos ficam vazios pois o controle é feito via 'op' dentro do loop
    for (char op = 's'; op == 's' || op == 'S'; )
    {
        double g[4][5]; // matriz p guardar o gasto de cada setor por dia
        
        string ns[4] = {
            "1. Administração", "2. Informática", "3. Produção", "4. Almoxarifado"
        }; // vetor com os nomes dos 4 setores

        double ts[4] = {0, 0, 0, 0}; // acumula o gasto total de cada setor
        double td[5] = {0, 0, 0, 0, 0}; // acumula o gasto total de cada dia
        double te = 0; // acumula o consumo total geral da empresa

        int cse = 0; // conta quantos setores ficaram com consumo elevado
        int cd80 = 0; // conta quantos dias passaram de 80 kwh

        // loop dos setores e dias com confirmacao
        for (int i = 0; i < 4; i++)
        {
            // limpa a tela so quando muda de setor e exibe painel bonito
            system("cls");
            cout << "\033[36m+------------------------------------------------------+\033[0m" << endl;
            cout << "\033[36m¦\033[1m         ? DIAGNOSTICADOR ENERGÉTICO 2000 ?           \033[0m\033[36m¦\033[0m" << endl;
            cout << "\033[36m¦------------------------------------------------------¦\033[0m" << endl;
            cout << "\033[36m¦\033[0m Setor Atual: \033[1m" << ns[i] << "\033[0m" << endl;
            cout << "\033[36m+------------------------------------------------------+\033[0m" << endl;

            for (int j = 0; j < 5; j++)
            {
                char c = 'n'; // confirma se o valor digitado ta certo

                // fica no loop do dia ate confirmar o valor certo
                while (c != 's' && c != 'S')
                {
                    cout << "\n \033[33m[Dia " << j + 1 << "]\033[0m Digite o consumo (kWh): ";
                    cin >> g[i][j];

                    // valida se e negativo ou deu pau no cin
                    while (cin.fail() || g[i][j] < 0)
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << " \033[31m[Erro]\033[0m Valor inválido! Digite maior ou igual a 0: ";
                        cin >> g[i][j];
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    // confirma o valor digitado
                    cout << " ? Confirma \033[1m" << g[i][j] << " kWh\033[0m para o Dia " << j + 1 << "? (s/n): ";
                    cin >> c;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (c != 's' && c != 'S' && c != 'n' && c != 'N')
                    {
                        cout << " \033[31m[Aviso]\033[0m Opção inválida! Digite 's' ou 'n'." << endl;
                        c = 'n';
                    }
                }
                
                cout << " \033[32m? [Salvo com sucesso]\033[0m" << endl;

                // soma nos acumuladores sem usar o +=
                ts[i] = ts[i] + g[i][j];
                td[j] = td[j] + g[i][j];
                te = te + g[i][j];
            }

            // status parcial do setor em formato de card
            cout << "\n\033[32m+------------------------------------------------------+\033[0m" << endl;
            cout << "\033[32m¦\033[1m                STATUS PARCIAL DO SETOR               \033[0m\033[32m¦\033[0m" << endl;
            cout << "\033[32m¦------------------------------------------------------¦\033[0m" << endl;
            cout << " Total do Setor : " << ts[i] << " kWh" << endl;
            cout << " Classificação  : ";
            if (ts[i] >= 100) {
                cout << "\033[31m\033[1mCONSUMO ELEVADO\033[0m" << endl;
            } else {
                cout << "\033[32m\033[1mCONSUMO NORMAL\033[0m" << endl;
            }
            cout << "\033[32m+------------------------------------------------------+\033[0m" << endl;
            
            if (i < 3) {
                cout << "\n Pressione ENTER para ir para o próximo setor...";
            } else {
                cout << "\n Pressione ENTER para gerar o relatório final completo...";
            }
            cin.get();
        }

        // contadores finais (>= 100)
        for (int i = 0; i < 4; i++)
        {
            if (ts[i] >= 100) 
            {
                cse = cse + 1;
            }
        }

        for (int j = 0; j < 5; j++)
        {
            if (td[j] > 80)
            {
                cd80 = cd80 + 1;
            }
        }

        // acha o maior setor e o maior dia
        int ims = 0; // indice para o maior setor
        for (int i = 1; i < 4; i++)
        {
            if (ts[i] > ts[ims])
            {
                ims = i;
            }
        }

        int imd = 0; // indice para o maior dia
        for (int j = 1; j < 5; j++)
        {
            if (td[j] > td[imd])
            {
                imd = j;
            }
        }

        // relatorio geral em tabela estilizada
        system("cls");
        cout << "\033[35m+------------------------------------------------------+\033[0m" << endl;
        cout << "\033[35m¦\033[1m                  RELATÓRIO DE CONSUMO                \033[0m\033[35m¦\033[0m" << endl;
        cout << "\033[35m+------------------------------------------------------+\033[0m" << endl;
        cout << "\nSetores\t\tD1\tD2\tD3\tD4\tD5" << endl;
        cout << "-------------------------------------------------------" << endl;
        for (int i = 0; i < 4; i++)
        {
            cout << i + 1 << ". Setor\t";
            for (int j = 0; j < 5; j++)
            {
                cout << g[i][j] << "\t";
            }
            cout << endl;
        }
        cout << "-------------------------------------------------------" << endl;

        // totais por setor
        cout << "\n\033[1m--- TOTAL E CLASSIFICAÇÃO POR SETOR ---\033[0m" << endl;
        for (int i = 0; i < 4; i++)
        {
            cout << "• " << ns[i] << " ? " << ts[i] << " kWh | ";
            if (ts[i] >= 100) {
                cout << "\033[31mCONSUMO ELEVADO\033[0m" << endl;
            } else {
                cout << "\033[32mCONSUMO NORMAL\033[0m" << endl;
            }
        }

        // total por dia
        cout << "\n\033[1m--- CONSUMO TOTAL POR DIA ---\033[0m" << endl;
        for (int j = 0; j < 5; j++)
        {
            cout << "• Dia " << j + 1 << ": " << td[j] << " kWh" << endl;
        }

        // resumo final estilizado
        cout << "\n\033[36m+------------------------------------------------------+\033[0m" << endl;
        cout << "\033[36m¦\033[1m                     RESUMO GERAL                     \033[0m\033[36m¦\033[0m" << endl;
        cout << "\033[36m¦------------------------------------------------------¦\033[0m" << endl;
        cout << " Consumo Total da Empresa : " << te << " kWh" << endl;
        cout << " Setores com Consumo Elev.: " << cse << endl;
        cout << " Dias com Consumo > 80kWh : " << cd80 << endl;
        cout << "-------------------------------------------------------" << endl;
        cout << " ?? Maior Setor Consumidor: " << ns[ims] << " (" << ts[ims] << " kWh)" << endl;
        cout << " ?? Dia de Maior Pico     : Dia " << imd + 1 << " (" << td[imd] << " kWh)" << endl;
        cout << "\033[36m+------------------------------------------------------+\033[0m" << endl;

        cout << "\n";
        system("pause");

        // repetir o programa lendo a variável do for
        system("cls");
        cout << "Deseja rodar o programa novamente? (s/n): ";
        cin >> op;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    system("cls");
    cout << "\033[31m+------------------------------------------------------+\033[0m" << endl;
    cout << "\033[31m¦\033[1m              PROGRAMA ENCERRADO. ATÉ LOGO!           \033[0m\033[31m¦\033[0m" << endl;
    cout << "\033[31m+------------------------------------------------------+\033[0m" << endl;
    system("pause");
    return 0;
}
