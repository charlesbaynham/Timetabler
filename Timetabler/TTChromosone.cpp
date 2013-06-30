//
//  TTChromosone.cpp
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "TTChromosone.h"
#include <ext/hash_map>

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
        _values.resize( SLOTS_IN_DAY * Configuration::getInstance().numTutors() );
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
        Student* currStudent = *it;
        //choose random position:
        int pos;
        pos = GaGlobalRandomIntegerGenerator->Generate( SLOTS_IN_DAY * Configuration::getInstance().numTutors() - 1 );
        
        // Add the current student to the list at the location 'pos' in the _values vector
        newChromosone->_values.at(pos).push_back(currStudent);
        
        // Add the (student*,position) pair to the hashmap
        newChromosone->_lookup.insert(pair<Student*, int>(currStudent,pos));
        
    }
    
    //debug
//    for (hash_map<Student*,int>::const_iterator it = newChromosone->_lookup.begin(); it != newChromosone->_lookup.end(); it++) {
//        pair<Student*, int> thepair = *it;
//        string name = thepair.first->getName();
//        int num = thepair.second;
//        cout << name << "\t\tis at\t\t" << num << endl;
//    }
    printf("After creation, table has %i students\n", (int)newChromosone->_lookup.size());
    //end debug
    
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
    
    //debug
    printf("Mutation: lookup size is %i\n", numStudents);
    
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
        //chromo->_lookup = lookup;
    }
    
    numStudents = (int)chromo->_lookup.size();
    printf("Mutation: size = %i\n", numStudents);
}

float TTFitness::operator()(const GaChromosome* chromosome) const{
    const Chromosone* chromo = dynamic_cast<const Chromosone*>(chromosome);
    
    int score = 0;
    int numStudents = (int)chromo->_lookup.size();
    int numSlots = SLOTS_IN_DAY * Configuration::getInstance().numTutors();
    
    
    //debug
    printf("Fitness: lookup size is %i\n", numStudents);
    //end debug
    
    // loop over all students
    for (hash_map<Student*,int>::const_iterator it = chromo->_lookup.begin(); it!=chromo->_lookup.end(); it++ ) {
        
        //is there overlapping?
        if ( !(chromo->_values[(*it).second].size()>1) ) score++;
        
        //Does the tutor teach the subject?
        //get tutor:
        div_t division = div((*it).second, SLOTS_IN_DAY);
        int tutorID = division.quot;
        int time = division.rem;
        
        Tutor* tutor = Configuration::getInstance().getTutor(tutorID);
        
        //check subject:
        list<Subject*> tutSubj = tutor->getSujects();
        for (list<Subject*>::iterator itTut = tutSubj.begin(); itTut != tutSubj.end(); itTut++)
        {
            if ( (*it).first->getSubject() == *itTut )
            {
                score++;
                break; // Need to adapt to suit proficiency in subject. edit. 
            }
        }
        
        //can the tutor do the time?
        bool canDo = true;
        list<int> tutNotSlots = tutor->getNotSlots();
        for (list<int>::iterator itTut = tutNotSlots.begin(); itTut != tutNotSlots.end(); itTut++)
        {
            if ( *itTut == (*it).second )
            {
                canDo = false;
                break;
            }
        }
        if (canDo) score++;
    }
    
    int maxscore = 3 * numStudents;
    
    return (float)score / (float)maxscore;
}



GaChromosomePtr TTCrossover::operator ()(const GaChromosome* parent1, const GaChromosome* parent2) const
{
    const Chromosone* c1 = dynamic_cast<const Chromosone*>( parent1 );
    const Chromosone* c2 = dynamic_cast<const Chromosone*>( parent2 );
    
//    hash_map<Student*, int> c1map = c1->_lookup;
//    hash_map<Student*, int> c2map = c2->_lookup;
//    
////    //debug
////    for (hash_map<Student*,int>::const_iterator it = c1map.begin(); it != c1map.end(); it++) {
////        pair<Student*, int> thepair = *it;
////        string name = thepair.first->getName();
////        int num = thepair.second;
////        cout << name << "  is at  " << num << endl;
////    }
////    //end debug
//    
//    Chromosone* n = new Chromosone(*c1, true);
//    hash_map<Student*, int> nmap = n->_lookup;
//    
//    // number of students
//	int size1 = (int)c1map.size();
//    int size2 = (int)c2map.size();
//    
//    printf("Crossover: sizes are %i and %i\n", size1, size2);
//    
//    int size = size1;
//    
//	// determine crossover point (randomly)
//	vector<bool> cp( size );
//	for( int i = c1->GetParameters().GetNumberOfCrossoverPoints(); i > 0; i-- )
//	{
//		while( 1 )
//		{
//			int p = GaGlobalRandomIntegerGenerator->Generate( size - 1 );
//			if( !cp[ p ] )
//			{
//				cp[ p ] = true;
//				break;
//			}
//		}
//	}
//    
//	hash_map<Student*,int>::const_iterator it1 = c1map.begin();
//	hash_map<Student*,int>::const_iterator it2 = c2map.begin();
//    
//	// make new code by combining parent codes
//	bool first = GaGlobalRandomBoolGenerator->Generate();
//	for( int i = 0; i < size; i++ )
//	{
//		if( first )
//		{
//			// insert class from first parent into new chromosome's class table
//			nmap.insert( pair<Student*, int>( ( *it1 ).first, ( *it1 ).second ) );
//			// add to corresponding slot
//			n->_values[ ( *it1 ).second ].push_back( ( *it1 ).first );
//		}
//		else
//		{
//			// insert class from second parent into new chromosome's class table
//			nmap.insert( pair<Student*, int>( ( *it2 ).first, ( *it2 ).second ) ); // here's a crash
//			// add to corresponding slot
//			n->_values[ ( *it2 ).second ].push_back( ( *it2 ).first );
//		}
//        
//		// crossover point
//		if( cp[ i ] )
//			// change source chromosome if chosen earlier
//			first = !first;
//
//		it2++; // here's another
//		it1++; // but not here...
//	}
//    
//    size1 = (int)c1map.size();
//    size2 = (int)c2map.size();
//    
//    printf("Crossover end: sizes are %i and %i\n", size1, size2);
    
    
    //DEBUG
    Chromosone* n = new Chromosone(*c1, false);
    return n;
}



