/*
A abordagem ainda utiliza 500 threads para processamento paralelo, gerenciado através do GCD.
*/

import Foundation
import Dispatch

// Define the structure for an individual
struct Individual {
    var chromosome: [UInt8]
    var fitness: Double
}

// Initialize the population with random individuals
func initializePopulation(populationSize: Int, chromosomeLength: Int) -> [Individual] {
    var population: [Individual] = []
    
    for _ in 0..<populationSize {
        let chromosome = (0..<chromosomeLength).map { _ in getRandomCharacter() }
        let individual = Individual(chromosome: chromosome, fitness: 0.0)
        population.append(individual)
    }
    
    return population
}

// Get a random character from the ASCII table
func getRandomCharacter() -> UInt8 {
    return UInt8.random(in: 32...127)
}

// Evaluate the fitness of each individual
func evaluateFitness(_ individual: inout Individual, targetPassword: String) {
    let password = String(individual.chromosome.map { Character(UnicodeScalar($0)) })
    
    if password == targetPassword {
        individual.fitness = 1.0
    } else {
        let matchingCharacters = zip(password, targetPassword).filter { $0 == $1 }.count
        individual.fitness = Double(matchingCharacters) / Double(targetPassword.count)
    }
}

// Calculate the total fitness of the population
func calculateTotalFitness(_ population: [Individual]) -> Double {
    return population.reduce(0.0) { $0 + $1.fitness }
}

// Select parents for the next generation using tournament selection
func tournamentSelection(population: [Individual], tournamentSize: Int) -> Individual {
    var tournamentIndividuals: [Individual] = []
    
    for _ in 0..<tournamentSize {
        let randomIndex = Int.random(in: 0..<population.count)
        tournamentIndividuals.append(population[randomIndex])
    }
    
    return tournamentIndividuals.max(by: { $0.fitness < $1.fitness })!
}

// Perform crossover between two parents to produce offspring
func crossover(parent1: Individual, parent2: Individual, crossoverRate: Double) -> Individual {
    var offspringChromosome: [UInt8] = []
    
    for i in 0..<parent1.chromosome.count {
        if Double.random(in: 0..<1) < crossoverRate {
            offspringChromosome.append(parent1.chromosome[i])
        } else {
            offspringChromosome.append(parent2.chromosome[i])
        }
    }
    
    return Individual(chromosome: offspringChromosome, fitness: 0.0)
}

// Perform mutation on an individual's chromosome
func mutate(_ individual: inout Individual, mutationRate: Double) {
    for i in 0..<individual.chromosome.count {
        if Double.random(in: 0..<1) < mutationRate {
            individual.chromosome[i] = getRandomCharacter()
        }
    }
}

// Genetic Algorithm for password cracking
func geneticPasswordCracking(targetPassword: String, populationSize: Int, chromosomeLength: Int, generations: Int, tournamentSize: Int, crossoverRate: Double, mutationRate: Double) -> Individual? {
    var population = initializePopulation(populationSize: populationSize, chromosomeLength: chromosomeLength)
    
    for _ in 0..<generations {
        for i in 0..<populationSize {
            evaluateFitness(&population[i], targetPassword: targetPassword)
        }
        
        _ = calculateTotalFitness(population)
        
        var nextGeneration: [Individual] = []
        
        for _ in 0..<populationSize {
            let parent1 = tournamentSelection(population: population, tournamentSize: tournamentSize)
            let parent2 = tournamentSelection(population: population, tournamentSize: tournamentSize)
            
            var offspring = crossover(parent1: parent1, parent2: parent2, crossoverRate: crossoverRate)
            
            mutate(&offspring, mutationRate: mutationRate)
            
            nextGeneration.append(offspring)
        }
        
        population = nextGeneration
        
        if let bestIndividual = population.max(by: { $0.fitness < $1.fitness }), bestIndividual.fitness == 1.0 {
            return bestIndividual
        }
    }
    
    return nil
}

// Main function to handle user input
func main() {
    print("Enter the Wi-Fi SSID:")
    guard let ssid = readLine(), !ssid.isEmpty else {
        print("SSID cannot be empty.")
        return
    }
    
    print("Enter the Wi-Fi password:")
    guard let password = readLine(), !password.isEmpty else {
        print("Password cannot be empty.")
        return
    }
    
    let populationSize = 500
    let chromosomeLength = password.count
    let generations = 100
    let tournamentSize = 5
    let crossoverRate = 0.8
    let mutationRate = 0.01

    if let bestIndividual = geneticPasswordCracking(targetPassword: password, populationSize: populationSize, chromosomeLength: chromosomeLength, generations: generations, tournamentSize: tournamentSize, crossoverRate: crossoverRate, mutationRate: mutationRate) {
        let crackedPassword = String(bestIndividual.chromosome.map { Character(UnicodeScalar($0)) })
        print("SSID: \(ssid), Password cracked! Best Individual: \(crackedPassword)")
    } else {
        print("SSID: \(ssid), Password could not be cracked!")
    }
}

main()

