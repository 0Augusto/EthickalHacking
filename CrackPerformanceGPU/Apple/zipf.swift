/*
m Swift, utilizamos a função pow() para calcular a potência, e Double.random(in: Range) para gerar um valor aleatório dentro de um determinado intervalo.
A função srand() é substituída pelo uso de srand(UInt32(time(nil))) para inicializar o gerador de números aleatórios com base no tempo atual.
Ao invés de usar um array de tamanho fixo como em C, em Swift usamos um array dinâmico (uma lista) para armazenar os elementos da sequência gerada.
Essas são as principais adaptações feitas para reescrever o código em Swift.

A ordem de complexidade (complexidade assintótica) é O(k), onde k é o comprimento da sequência a ser gerada (sequenceLength). Isso ocorre porque a função generateZipfSequence(length:) possui um loop for que itera length vezes, e dentro desse loop, existe outro loop while, que também pode iterar várias vezes dependendo do valor de randomValue e da distribuição Zipf.

Em relação ao valor de N (100, definido no código original), ele não afeta diretamente a complexidade do código, pois ele é usado apenas para calcular a função harmônica geral na função generalizedHarmonic(_:_:), que tem uma complexidade O(N) devido ao loop for que itera de 1 até N.

Dentro do contexto do problema específico e da distribuição Zipf, a complexidade O(k) é aceitável, pois estamos gerando uma sequência de tamanho específico e não precisamos calcular a função harmônica completa para cada elemento da sequência. No entanto, se o tamanho da sequência ou o valor de N forem significativamente grandes, a complexidade pode se tornar relevante. Nesses casos, otimizações específicas podem ser aplicadas para melhorar o desempenho do código.
*/

import Foundation

let N = 100 // Número total de elementos
let S = 1.0 // Parâmetro de inclinação da distribuição Zipf

// Função para calcular a função harmônica generalizada H(N, s)
func generalizedHarmonic(_ n: Int, _ s: Double) -> Double {
    var sum = 0.0
    for i in 1...n {
        sum += 1.0 / pow(Double(i), s)
    }
    return sum
}

// Função para gerar uma sequência de elementos com base na distribuição Zipf
func generateZipfSequence(length: Int) -> [Int] {
    var sequence = [Int]()
    let harmonic = generalizedHarmonic(N, S) // Calcula a função harmônica geral com base em N e S
    for _ in 0..<length {
        let randomValue = Double.random(in: 0..<1) // Gera um valor aleatório entre 0 e 1
        var cumulativeProbability = 0.0
        var element = 1
        while cumulativeProbability < randomValue {
            cumulativeProbability += (1.0 / (pow(Double(element), S) * harmonic)) // Calcula a probabilidade cumulativa para o elemento atual
            element += 1
        }
        sequence.append(element - 1) // Armazena o elemento gerado na sequência (subtraindo 1 para ajustar à base 0)
    }
    return sequence
}

srand(UInt32(time(nil))) // Inicializa o gerador de números aleatórios com base no tempo atual

let sequenceLength = 10 // Comprimento da sequência a ser gerada
let sequence = generateZipfSequence(length: sequenceLength) // Gera a sequência de elementos com base na distribuição Zipf

print("Sequência gerada: \(sequence)")

