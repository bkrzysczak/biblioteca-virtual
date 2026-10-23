#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "banco.h"

void exibirMenu() {
    std::cout << "\n========== GERENCIADOR DE BIBLIOTECA ==========" << std::endl;
    std::cout << "1. Adicionar novo livro" << std::endl;
    std::cout << "2. Remover livro" << std::endl;
    std::cout << "3. Emprestar livro" << std::endl;
    std::cout << "4. Devolver livro" << std::endl;
    std::cout << "5. Listar todos os livros" << std::endl;
    std::cout << "6. Buscar livro por nome" << std::endl;
    std::cout << "7. Buscar livro por autor" << std::endl;
    std::cout << "8. Sair" << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << "Escolha uma opção: ";
}

void exibirLivros(const std::vector<Livro>& livros) {
    if (livros.empty()) {
        std::cout << "Nenhum livro encontrado!" << std::endl;
        return;
    }

    std::cout << "\n" << std::setw(5) << "ID" 
              << std::setw(25) << "Nome" 
              << std::setw(20) << "Autor" 
              << std::setw(15) << "ISBN" 
              << std::setw(12) << "Total" 
              << std::setw(12) << "Emprestados"
              << std::setw(12) << "Disponíveis" << std::endl;
    std::cout << std::string(101, '-') << std::endl;

    for (const auto& livro : livros) {
        std::cout << std::setw(5) << livro.getId()
                  << std::setw(25) << livro.getNome().substr(0, 24)
                  << std::setw(20) << livro.getAutor().substr(0, 19)
                  << std::setw(15) << livro.getIsbn()
                  << std::setw(12) << livro.getQuantidade()
                  << std::setw(12) << livro.getEmprestados()
                  << std::setw(12) << livro.getDisponiveis() << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    // ===== INICIALIZAÇÃO DO BANCO =====
    std::cout << "Inicializando o gerenciador de biblioteca..." << std::endl;
    
    Banco banco("biblioteca.db");  // Cria/conecta ao arquivo biblioteca.db
    
    if (!banco.conectar()) {
        std::cerr << "Erro ao conectar ao banco de dados!" << std::endl;
        return 1;
    }
    
    if (!banco.criarTabelas()) {  // Cria a tabela se não existir
        std::cerr << "Erro ao criar as tabelas!" << std::endl;
        return 1;
    }
    
    std::cout << "Banco de dados inicializado com sucesso!\n" << std::endl;

    // ===== LOOP PRINCIPAL =====
    int opcao = 0;
    
    while (true) {
        exibirMenu();
        std::cin >> opcao;
        std::cin.ignore();  // Limpa o buffer do teclado

        switch (opcao) {
            case 1: {  // Adicionar livro
                std::cout << "\n--- Adicionar Novo Livro ---" << std::endl;
                
                std::string nome, autor, isbn;
                int quantidade;
                
                std::cout << "Nome do livro: ";
                std::getline(std::cin, nome);
                
                std::cout << "Autor: ";
                std::getline(std::cin, autor);
                
                std::cout << "ISBN: ";
                std::getline(std::cin, isbn);
                
                std::cout << "Quantidade: ";
                std::cin >> quantidade;
                std::cin.ignore();
                
                banco.adicionarLivro(nome, autor, isbn, quantidade);
                break;
            }

            case 2: {  // Remover livro
                std::cout << "\n--- Remover Livro ---" << std::endl;
                std::cout << "Digite o ID do livro a remover: ";
                int id;
                std::cin >> id;
                std::cin.ignore();
                
                banco.removerLivro(id);
                break;
            }

            case 3: {  // Emprestar livro
                std::cout << "\n--- Emprestar Livro ---" << std::endl;
                std::cout << "Digite o ID do livro a emprestar: ";
                int id;
                std::cin >> id;
                std::cin.ignore();
                
                banco.emprestarLivro(id);
                break;
            }

            case 4: {  // Devolver livro
                std::cout << "\n--- Devolver Livro ---" << std::endl;
                std::cout << "Digite o ID do livro a devolver: ";
                int id;
                std::cin >> id;
                std::cin.ignore();
                
                banco.devolverLivro(id);
                break;
            }

            case 5: {  // Listar todos
                std::cout << "\n--- Listando Todos os Livros ---" << std::endl;
                auto livros = banco.listarTodosLivros();
                exibirLivros(livros);
                break;
            }

            case 6: {  // Buscar por nome
                std::cout << "\n--- Buscar Livro por Nome ---" << std::endl;
                std::cout << "Digite o nome (ou parte dele): ";
                std::string nome;
                std::getline(std::cin, nome);
                
                auto livros = banco.buscarLivroPorNome(nome);
                exibirLivros(livros);
                break;
            }

            case 7: {  // Buscar por autor
                std::cout << "\n--- Buscar Livro por Autor ---" << std::endl;
                std::cout << "Digite o autor (ou parte dele): ";
                std::string autor;
                std::getline(std::cin, autor);
                
                auto livros = banco.buscarLivroPorAutor(autor);
                exibirLivros(livros);
                break;
            }

            case 8: {  // Sair
                std::cout << "Encerrando o programa..." << std::endl;
                banco.desconectar();
                return 0;
            }

            default:
                std::cout << "Opção inválida! Tente novamente." << std::endl;
        }
    }

    return 0;
}
