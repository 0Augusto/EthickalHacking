/*
Observe que o código em Swift usa a função readLine() para ler a entrada do usuário e fgets() para ler as senhas do arquivo. Além disso, a estrutura ThreadArgs é usada para passar os argumentos para a função da thread em Swift. A criação e manipulação de threads também são diferentes em Swift em comparação com C.
*/

import Foundation

let NUM_THREADS = 500

let correctPassword = "mySecretPassword"
var passwordFound = false

// Estrutura para armazenar os argumentos da função da thread
struct ThreadArgs {
    let password: String
    let start: Int
    let end: Int
}

// Função de verificação de senha executada pela thread
func checkPassword(args: UnsafeMutableRawPointer?) -> UnsafeMutableRawPointer? {
    guard let args = args else {
        return nil
    }

    let threadArgs = args.assumingMemoryBound(to: ThreadArgs.self)
    let password = threadArgs.pointee.password
    let start = threadArgs.pointee.start
    let end = threadArgs.pointee.end

    for i in start..<end {
        if password == correctPassword {
            print("Senha encontrada: \(password)")
            passwordFound = true
            break
        }
    }

    return nil
}

func main() {
    var file: UnsafeMutablePointer<FILE>?
    var filename = [CChar](repeating: 0, count: 100)
    var line = [CChar](repeating: 0, count: 100)

    print("Digite o nome do arquivo com as senhas a serem verificadas: ", terminator: "")
    guard let input = readLine() else {
        return
    }
    filename = input.cString(using: .utf8)!

    // Abrir o arquivo
    file = fopen(filename, "r")
    if file == nil {
        print("Não foi possível abrir o arquivo \(String(cString: filename)).")
        return
    }

    // Ler senhas do arquivo e testar
    while fgets(&line, 100, file) != nil {
        line[Int(strcspn(line, "\n"))] = 0 // Remover o caractere de nova linha (\n) do final

        let password = String(cString: line)
        let passwordLength = password.count
        let chunkSize = passwordLength / NUM_THREADS

        var threads = [pthread_t]()
        var threadArgs = [ThreadArgs]()

        for i in 0..<NUM_THREADS {
            let start = i * chunkSize
            let end = (i == NUM_THREADS - 1) ? passwordLength : (i + 1) * chunkSize
            let args = ThreadArgs(password: password, start: start, end: end)
            threadArgs.append(args)

            var thread: pthread_t = pthread_t()
            threads.append(thread)

            pthread_create(&threads[i], nil, checkPassword, UnsafeMutableRawPointer(mutating: &threadArgs[i]))
        }

        for thread in threads {
            pthread_join(thread, nil)
        }

        if passwordFound {
            print("Senha encontrada: \(password)")
            break
        }
    }

    fclose(file)

    if !passwordFound {
        print("Nenhuma senha encontrada.")
    }
}

main()

