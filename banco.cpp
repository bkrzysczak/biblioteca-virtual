#include "banco.h"
#include <iostream>
#include <sstream>

Banco::Banco(const std::string& caminho) 
    : db(nullptr), caminhoDb(caminho) {}

Banco::~Banco() {
    desconectar();
}

bool Banco::conectar() {
    int resultado = sqlite3_open(caminhoDb.c_str(), &db);
    if (resultado != SQLITE_OK) {
        std::cerr << "Erro ao conectar ao banco de dados: " 
                  << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    std::cout << "Conectado ao banco de dados com sucesso!" << std::endl;
    return true;
}

bool Banco::desconectar() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
        std::cout << "Desconectado do banco de dados." << std::endl;
    }
    return true;
}

bool Banco::criarTabelas() {
    std::string sql = R"(
        CREATE TABLE IF NOT EXISTS livros (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            nome TEXT NOT NULL,
            autor TEXT NOT NULL,
            isbn TEXT UNIQUE NOT NULL,
            quantidade INTEGER NOT NULL,
            emprestados INTEGER DEFAULT 0,
            data_criacao DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";

    return executarSQL(sql);
}

bool Banco::executarSQL(const std::string& sql) {
    char* erroMsg = nullptr;
    int resultado = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &erroMsg);
    
    if (resultado != SQLITE_OK) {
        std::cerr << "Erro ao executar SQL: " << erroMsg << std::endl;
        sqlite3_free(erroMsg);
        return false;
    }
    return true;
}

bool Banco::adicionarLivro(const std::string& nome, const std::string& autor,
                           const std::string& isbn, int quantidade) {
    std::ostringstream sql;
    sql << "INSERT INTO livros (nome, autor, isbn, quantidade) VALUES ("
        << "'" << nome << "', "
        << "'" << autor << "', "
        << "'" << isbn << "', "
        << quantidade << ");";

    if (executarSQL(sql.str())) {
        std::cout << "Livro adicionado com sucesso!" << std::endl;
        return true;
    }
    return false;
}

bool Banco::removerLivro(int id) {
    std::ostringstream sql;
    sql << "DELETE FROM livros WHERE id = " << id << ";";

    if (executarSQL(sql.str())) {
        int linhasAfetadas = sqlite3_changes(db);
        if (linhasAfetadas > 0) {
            std::cout << "Livro removido com sucesso!" << std::endl;
            return true;
        } else {
            std::cerr << "Erro: Nenhum livro encontrado com o ID " << id << "!" << std::endl;
            return false;
        }
    }
    return false;
}

bool Banco::atualizarLivro(int id, const std::string& nome, 
                           const std::string& autor, const std::string& isbn, 
                           int quantidade) {
    std::ostringstream sql;
    sql << "UPDATE livros SET nome = '" << nome 
        << "', autor = '" << autor 
        << "', isbn = '" << isbn 
        << "', quantidade = " << quantidade 
        << " WHERE id = " << id << ";";

    if (executarSQL(sql.str())) {
        int linhasAfetadas = sqlite3_changes(db);
        if (linhasAfetadas > 0) {
            std::cout << "Livro atualizado com sucesso!" << std::endl;
            return true;
        } else {
            std::cerr << "Erro: Nenhum livro encontrado com o ID " << id << "!" << std::endl;
            return false;
        }
    }
    return false;
}

bool Banco::emprestarLivro(int id) {
    Livro livro = buscarLivroPorId(id);
    
    if (livro.getId() == 0) {
        std::cerr << "Livro não encontrado!" << std::endl;
        return false;
    }
    
    if (livro.getDisponiveis() <= 0) {
        std::cerr << "Nenhuma cópia disponível para empréstimo!" << std::endl;
        return false;
    }

    std::ostringstream sql;
    sql << "UPDATE livros SET emprestados = emprestados + 1 WHERE id = " 
        << id << ";";

    if (executarSQL(sql.str())) {
        std::cout << "Livro emprestado com sucesso!" << std::endl;
        return true;
    }
    return false;
}

bool Banco::devolverLivro(int id) {
    Livro livro = buscarLivroPorId(id);
    
    if (livro.getId() == 0) {
        std::cerr << "Livro não encontrado!" << std::endl;
        return false;
    }
    
    if (livro.getEmprestados() <= 0) {
        std::cerr << "Este livro não possui cópias emprestadas!" << std::endl;
        return false;
    }

    std::ostringstream sql;
    sql << "UPDATE livros SET emprestados = emprestados - 1 WHERE id = " 
        << id << ";";

    if (executarSQL(sql.str())) {
        std::cout << "Livro devolvido com sucesso!" << std::endl;
        return true;
    }
    return false;
}

std::vector<Livro> Banco::listarTodosLivros() {
    std::vector<Livro> livros;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, nome, autor, isbn, quantidade, emprestados FROM livros;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Livro livro;
            livro.setId(sqlite3_column_int(stmt, 0));
            livro.setNome(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            livro.setAutor(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            livro.setIsbn(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
            livro.setQuantidade(sqlite3_column_int(stmt, 4));
            livro.setEmprestados(sqlite3_column_int(stmt, 5));
            livros.push_back(livro);
        }
    }
    sqlite3_finalize(stmt);
    return livros;
}

Livro Banco::buscarLivroPorId(int id) {
    Livro livro;
    sqlite3_stmt* stmt;
    std::ostringstream sql;
    sql << "SELECT id, nome, autor, isbn, quantidade, emprestados FROM livros WHERE id = " << id << ";";

    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            livro.setId(sqlite3_column_int(stmt, 0));
            livro.setNome(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            livro.setAutor(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            livro.setIsbn(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
            livro.setQuantidade(sqlite3_column_int(stmt, 4));
            livro.setEmprestados(sqlite3_column_int(stmt, 5));
        }
    }
    sqlite3_finalize(stmt);
    return livro;
}

std::vector<Livro> Banco::buscarLivroPorNome(const std::string& nome) {
    std::vector<Livro> livros;
    sqlite3_stmt* stmt;
    std::ostringstream sql;
    sql << "SELECT id, nome, autor, isbn, quantidade, emprestados FROM livros WHERE nome LIKE '%" << nome << "%';";

    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Livro livro;
            livro.setId(sqlite3_column_int(stmt, 0));
            livro.setNome(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            livro.setAutor(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            livro.setIsbn(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
            livro.setQuantidade(sqlite3_column_int(stmt, 4));
            livro.setEmprestados(sqlite3_column_int(stmt, 5));
            livros.push_back(livro);
        }
    }
    sqlite3_finalize(stmt);
    return livros;
}

std::vector<Livro> Banco::buscarLivroPorAutor(const std::string& autor) {
    std::vector<Livro> livros;
    sqlite3_stmt* stmt;
    std::ostringstream sql;
    sql << "SELECT id, nome, autor, isbn, quantidade, emprestados FROM livros WHERE autor LIKE '%" << autor << "%';";

    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Livro livro;
            livro.setId(sqlite3_column_int(stmt, 0));
            livro.setNome(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            livro.setAutor(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            livro.setIsbn(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
            livro.setQuantidade(sqlite3_column_int(stmt, 4));
            livro.setEmprestados(sqlite3_column_int(stmt, 5));
            livros.push_back(livro);
        }
    }
    sqlite3_finalize(stmt);
    return livros;
}

