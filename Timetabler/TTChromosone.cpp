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
        //choose random position:
        int pos;
        pos = GaGlobalRandomIntegerGenerator->Generate( SLOTS_IN_DAY * Configuration::getInstance().numTutors() - 1 );
        
        // Add the current student to the list at the location 'pos' in the _values vector
        newChromosone->_values.at(pos).push_back(*it);
        
        // Add the (student*,position) pair to the hashmap
        printf("pair will be studentid=%i, poss=%i\n", (*it)->getID(), pos);
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
    printf("Doing mutation.\n");
    Chromosone* chromo = dynamic_cast<Chromosone*>(parent);
    
    int numSlots = (int)chromo->_values.size();
    int numStudents = (int)chromo->_lookup.size();
    hash_map<Student*,int> lookup = chromo->GetStudentLookup();
    
    //for each mutation:
    for (int i = chromo->GetParameters().GetMutationSize() ; i>0; i--)
    {
        // pick a random student
        int student = GaGlobalRandomIntegerGenerator->Generate(numStudents-1);
        hash_map<Student*, int>::iterator it;
        
        // Iterate through the hashmap _lookup, reducing student and increasing it until student == 0:
        //   now it points to a randomly chosen student.
        for (it = lookup.begin();
             student != 0 && it != lookup.end();
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
        lookup[theStudent] = newSlot;
        chromo->_lookup = lookup;
    }
    
    
}

float TTFitness::operator()(const GaChromosome* chromosome) const{
    printf("Doing fitness.\n");
    const Chromosone* chromo = dynamic_cast<const Chromosone*>(chromosome);
    
    int score = 0;
    int numStudents = (int)chromo->_lookup.size();
    int numSlots = SLOTS_IN_DAY * Configuration::getInstance().numTutors();
    
    // loop over all students
        printf("Crash?\n");
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
    printf("Doing crossover.\n");
    const Chromosone* c1 = dynamic_cast<const Chromosone*>( parent1 );
    const Chromosone* c2 = dynamic_cast<const Chromosone*>( parent2 );
    
    Chromosone* n = new Chromosone(*c1, true);
    
    // number of students
	int size = (int)c1->_lookup.size();
    
	// determine crossover point (randomly)
	vector<bool> cp( size );
	for( int i = c1->GetParameters().GetNumberOfCrossoverPoints(); i > 0; i-- )
	{
		while( 1 )
		{
			int p = GaGlobalRandomIntegerGenerator->Generate( size - 1 );
			if( !cp[ p ] )
			{
				cp[ p ] = true;
				break;
			}
		}
	}
    
	hash_map<Student*,int>::const_iterator it1 = c1->_lookup.begin();
	hash_map<Student*,int>::const_iterator it2 = c2->_lookup.begin();
    
	// make new code by combining parent codes
	bool first = GaGlobalRandomBoolGenerator->Generate();
	for( int i = 0; i < size; i++ )
	{
		if( first )
		{
			// insert class from first parent into new chromosome's class table
			n->_lookup.insert( pair<Student*, int>( ( *it1 ).first, ( *it1 ).second ) );
			// add to corresponding slot
			n->_values[ ( *it1 ).second ].push_back( ( *it1 ).first );
		}
		else
		{
			// insert class from second parent into new chromosome's class table
			n->_lookup.insert( pair<Student*, int>( ( *it2 ).first, ( *it2 ).second ) );
			// add to corresponding slot
			n->_values[ ( *it2 ).second ].push_back( ( *it2 ).first );
		}
        
		// crossover point
		if( cp[ i ] )
			// change source chromosome if chosen earlier
			first = !first;
        
		it1++;
		it2++;
	}
    
    return n;
}



