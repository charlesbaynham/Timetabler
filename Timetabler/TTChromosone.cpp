//
//  TTChromosone.cpp
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "TTChromosone.h"

Chromosone::Chromosone(GaChromosomeDomainBlock<list<Student*> >* configBlock) :
    GaMultiValueChromosome<list<Student*> >(configBlock)
{
    _values.resize( SLOTS_IN_DAY * Configuration::getInstance().numTutors() );
}

Chromosone::Chromosone(const Chromosone& c, bool setupOnly) :
    GaMultiValueChromosome<list<Student*> >(c, setupOnly)
{
    if (!setupOnly) { // Then copy the data
        _values = c._values;
        _lookup = c._lookup;
        _backupLookup = c._backupLookup;
    }
    else { // Reserve space
        _values.resize( SLOTS_IN_DAY * Configuration::getInstance().numTutors() ); //Get RID of num_tutors
    }
}

GaChromosomePtr Chromosone::MakeCopy(bool setupOnly) const { return new Chromosone( *this, setupOnly ); }

//Create a new, random chromosome using *this as a prototype for the setup
GaChromosomePtr Chromosone::MakeNewFromPrototype() const {
    
    // Make new chromosone copying setup
    Chromosone* newChromosone = new Chromosone(*this, true);
    
    //Get all the students:
    const list<Student*>& students = Configuration::getInstance().getStudents();
    
    //loop all over students
    for (list<Student*>::const_iterator it = students.begin(); it != students.end(); it++)
    {
        //choose random position:
        int pos;
        pos = GaGlobalRandomIntegerGenerator->Generate( SLOTS_IN_DAY * Configuration::getInstance().numTutors() - 1 );
        
        // Add the current student to the list at the location 'pos' in the _values vector
        newChromosone->_values.at(pos).push_back(*it);
        
        // Add the (student*,position) pair to the hashmap
        newChromosone->_lookup.insert(pair<Student*, int>(*it,pos));
    }
    
    return newChromosone;
}

void Chromosone::PreapareForMutation() {
    _backupLookup = _lookup; // Backup hashmap then call method to backup rest
   	GaMultiValueChromosome<list<Student*> >::PreapareForMutation();
}

void Chromosone::AcceptMutation() {
    _backupLookup.clear(); // Clear backup and accept
   	GaMultiValueChromosome<list<Student*> >::AcceptMutation();
}

void Chromosone::RejectMutation() {
    _lookup = _backupLookup; // Restore backup hashmap then call method to reject rest
    _backupLookup.clear();
   	GaMultiValueChromosome<list<Student*> >::RejectMutation();
}



// Randomly move some (mutation size) students to different (random) slots
void TTMutation::operator ()(GaChromosome* parent) const
{
    Chromosone* chromo = dynamic_cast<Chromosone*>(parent);
    
    int numSlots = (int)chromo->_values.size();
    int numStudents = (int)chromo->_lookup.size();
    
    //for each mutation:
    for (int i = chromo->GetParameters().GetMutationSize() ; i>0; i--)
    {
        // pick a random student
        int student = GaGlobalRandomIntegerGenerator->Generate(numStudents-1);
        hash_map<Student*, int>::iterator it;
        
        // Iterate through the hashmap _lookup, reducing student and increasing it until student == 0:
        //   now it points to a randomly chosen student. 
        for (it = chromo->_lookup.begin();
             student != 0 && it != chromo->_lookup.end();
             it++, student--) ;

        Student* theStudent = (*it).first;
        int oldSlot = (*it).second;

        //pick a random new slot:
        int newSlot = GaGlobalRandomIntegerGenerator->Generate(numSlots-1);
        
        //delete from old:
        list<Student*>& theOldSlot = chromo->_values[oldSlot];
        for (list<Student*>::iterator itOld = theOldSlot.begin(); itOld != theOldSlot.end(); itOld++) {
            if ( *itOld == theStudent )
            {
                theOldSlot.erase(itOld);
                break;
            }
        }
        
        //add to new:
        list<Student*>& theNewSlot = chromo->_values[newSlot];
        theNewSlot.push_back(theStudent);
        
        //update hashmap:
        chromo->_lookup[theStudent] = newSlot;
    }
    
    
}

float TTFitness::operator()(const GaChromosome* chromosome) const{
    
    const Chromosone* chromo = dynamic_cast<const Chromosone*>(chromosome);
    
    return chromo->GetFitness() + 1; // not finished edit.
}


GaChromosomePtr TTCrossover::operator ()(const GaChromosome* parent1, const GaChromosome* parent2) const
{
    const Chromosone* c1 = dynamic_cast<const Chromosone*>( parent1 );
    const Chromosone* c2 = dynamic_cast<const Chromosone*>( parent2 );
    
    Chromosone* n = new Chromosone(*c1, false);
    
    
    
    return n; //Obviously not finished.  EDIT
}



