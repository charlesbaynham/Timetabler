//
//  TTChromosone.cpp
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "TTChromosone.h"
#include <ext/hash_map>

void dumpHash (hash_map<Student*, int> in){
    for (hash_map<Student*, int>::iterator it = in.begin() ; it != in.end(); it++) {
        cout << (*it).first->getName() << "\tat " << (*it).second << endl;
    }
}

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
//    printf("After creation, table has %i students\n", (int)newChromosone->_lookup.size());
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
//    printf("Mutation: lookup size is %i\n", numStudents);
    
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
//    printf("Mutation: size = %i\n", numStudents);
}

float TTFitness::operator()(const GaChromosome* chromosome) const{
    const Chromosone* chromo = dynamic_cast<const Chromosone*>(chromosome);
    
    float score = 0;
    int numStudents = (int)chromo->_lookup.size();
    int numSlots = SLOTS_IN_DAY * Configuration::getInstance().numTutors();
    
    
    // loop over all students
    for (hash_map<Student*,int>::const_iterator it = chromo->_lookup.begin(); it!=chromo->_lookup.end(); it++ ) {
        
        //is there overlapping?
        if ( !(chromo->_values[(*it).second].size()>1) ) score+=1.5;
            // Overlapping is particularly bad, so should merit a higher penalty than other lacking major requirements, eg not teaching the subject
        
        //Does the tutor teach the subject?
        //get tutor:
        div_t division = div((*it).second, SLOTS_IN_DAY);
        int tutorID = division.quot + 1;
        int time = division.rem;
        
        Tutor* tutor = Configuration::getInstance().getTutor(tutorID);
        
        //check subject:
        list<Subject*> tutSubjs = tutor->getSubjects();
        for (list<Subject*>::iterator itTut = tutSubjs.begin(); itTut != tutSubjs.end(); itTut++)
        {
            Subject* studentSubject = (*it).first->getSubject();
            if ( studentSubject == *itTut )
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
        
        //is this student already busy at this time?
        int engagements = 0;
        //loop over all times (for one tutor)
        for (int i = time; i < numSlots; i += SLOTS_IN_DAY)
        {
            list<Student*> thisSlot= chromo->_values[i];
            list<Student*>::iterator search;
            for (search = thisSlot.begin(); search != thisSlot.end(); search++) {
                 // If we find a student with the same baseID (ie the same person) who has an appointment at this time:
                if ( (*search)->getBaseID() == (*it).first->getBaseID() ) engagements++;
            }
        }
        if (engagements==1) score++; // If we only found them once (ie in the slot we were considering) then score
        
        //  MINOR:
        //   Does this student/tutor pair appear elsewhere in this timetable?
        int pairings=0;
        //loop over all tutor's slots
        for (int i = SLOTS_IN_DAY*(tutorID-1); i < SLOTS_IN_DAY*tutorID; i++) {
            list<Student*> thisSlot= chromo->_values[i];
            list<Student*>::iterator search;
            for (search = thisSlot.begin(); search != thisSlot.end(); search++) {
                // If we find a student with the same baseID (ie the same person) paired with the same tutor:
                if ( (*search)->getBaseID() == (*it).first->getBaseID() ) pairings++;
            }
        }
        // This is a minor requirement, therefore, if the student is only paired with this tutor
        //   once then score a small prize, so other req. take precidence
        if (pairings==1) score+=0.2;
        
        //  MINOR:
        //   Has the tutor been seen previously in another session?
        bool seenPrev = false;
        list<Tutor*> prevTutors = (*it).first->getPrevTutors();
        for (list<Tutor*>::iterator itPrev = prevTutors.begin(); itPrev != prevTutors.end(); itPrev++)
        {
            if ( *itPrev == tutor ) {
                seenPrev = true;
                break;
            }
        }
        if (!seenPrev) score+=0.1;
    }
    
    float maxscore = 4.8 * numStudents;
//    float maxscore = 4.5 * numStudents;
    
    return (float)score / (float)maxscore;
}



GaChromosomePtr TTCrossover::operator ()(const GaChromosome* parent1, const GaChromosome* parent2) const
{
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
    
    
    //copy hashmaps into vector so that we are sure to have both iterating in the same order
    vector< pair<Student*, int> > vec1;
    vector< pair<Student*, int> > vec2;
    
    for ( hash_map<Student*, int>::const_iterator it = c1->_lookup.begin(); it != c1->_lookup.end(); it++ ) {
        
        Student * stud;
        int id;
        
        stud = (*it).first;
        id = (*it).second;
        
        pair<Student*, int> Npair = *new pair<Student*, int>(stud, id);
        
        vec1.push_back(Npair);
    }
    for ( hash_map<Student*, int>::const_iterator it = c2->_lookup.begin(); it != c2->_lookup.end(); it++ ) {
        
        Student * stud;
        int id;
        
        stud = (*it).first;
        id = (*it).second;
        
        pair<Student*, int> Npair = *new pair<Student*, int>(stud, id);
        
        vec2.push_back(Npair);
    }
    
    //sort the vectors into same order (ascending by ID)
    sort(vec1.begin(), vec1.end());
    sort(vec2.begin(), vec2.end());

    
	// make new code by combining parent codes
	bool first = GaGlobalRandomBoolGenerator->Generate();
	for( int i = 0; i < size; i++ )
	{
        //debug
        int id1 = vec1[i].first->getID();
        int id2 = vec2[i].first->getID();
//
//        if (id1 != id2) {
//            printf("\nno1 with size %i:\n\n", (int)c1->_lookup.size());
//            dumpHash(c1->_lookup);
//            printf("\nno2 with size %i:\n\n", (int)c1->_lookup.size());
//            dumpHash(c2->_lookup);
//        }
//        //end debug
        
		if( first )
		{
			// insert student from first parent into new chromosome's hashmap
			n->_lookup.insert( pair<Student*, int>( vec1[i].first, vec1[i].second ) );
			// add to corresponding slot
			n->_values[ vec1[i].second ].push_back( vec1[i].first );
		}
		else
		{
			// insert student from second parent into new chromosome's hashmap
			n->_lookup.insert( pair<Student*, int>( vec2[i].first, vec2[i].second ) ); // here's a crash
			// add to corresponding slot
			n->_values[ vec2[i].second ].push_back( vec2[i].first );
		}
        
		// crossover point
		if( cp[ i ] )
			// change source chromosome
			first = !first;
	}

    return n;
}


