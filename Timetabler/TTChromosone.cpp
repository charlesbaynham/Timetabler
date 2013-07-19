//
//  TTChromosone.cpp
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "TTChromosone.h"
#include "TimetablerInst.h"
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
// Also, if we're using a previously found solution, add this solution in as the first chromosome
GaChromosomePtr Chromosone::MakeNewFromPrototype() const {
    
    // Make new chromosone copying setup
    Chromosone* newChromosone = new Chromosone(*this, true);
    
    bool addbest = !TimetablerInst::getInstance()->bestAdded() && Configuration::getInstance().prevSolutionLoaded();
    if ( addbest ) {
        
        // hashmap that links baseID -> list of the students with this baseID
        hash_map<int, list<Student*> > studentsById;
        // for each baseID in the prev solution
        vector<list<int> > prevSolution = Configuration::getInstance().getPrevSolution();
        
        // for each slot in the prev solution
        for (int i=0; i < prevSolution.size(); i++) {
            // for each student in this slot
            for (list<int>::iterator it = prevSolution[i].begin(); it!= prevSolution[i].end(); it++) {
                if ( studentsById[*it].empty() )
                    studentsById[*it] = Configuration::getInstance().getStudentsByBaseID(*it);
            }
        }
        
        // hashmap now contains all the students by baseID
        
        // for each slot in new chromosome
        for (int i=0; i < newChromosone->_values.size(); i++) {
            // for each student in the previous solution
            printf("i = %i\n",i);
            for (list<int>::iterator it=prevSolution[i].begin(); it!=prevSolution[i].end(); it++) {
                // add the first student with this baseID to the table:
                
                // get the student
                Student* student = studentsById[*it].front();
                
                // store in the chromosome
                newChromosone->_values[i].push_back( student );
                newChromosone->_lookup[ student ] = i;
                
                // remove this first student from the list, so that it is not added again
                studentsById[*it].erase(studentsById[*it].begin());
                
            }
        }
        
        //debug edit
        
        map<int, Student*> debug;
        
        cerr << "***\n\nHashmap dump after first student added\n\n";
        for (hash_map<Student*, int>::iterator it=newChromosone->_lookup.begin(); it!=newChromosone->_lookup.end(); it++) {
            
            debug[(*it).second] = (*it).first;
        }
        
        for (map<int, Student*>::iterator it=debug.begin(); it!=debug.end(); it++) {
            cerr << "\t" << (*it).first << " : " << (*it).second->getName() << " (";
            cerr << (*it).second->getBaseID() << ")" << endl;
        }
        //end debug
        
        // return the optimal chromosome
        return newChromosone;
    }

//   Else, if this isn't the first chromosome or we're not using a previous solution, randomise like normal
    
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
    
    const TTChromosomeParams* params = dynamic_cast<const TTChromosomeParams*>(&chromo->GetParameters());
    
    int numSlots = (int)chromo->_values.size();
    int numStudents = (int)chromo->_lookup.size();
    
    // Swap two students instead of doing a move with a probability of ProbSwap

    bool swap = GaGlobalRandomBoolGenerator->Generate( params->getProbSwap() );
    
    if (!swap) {
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
    } else {
        
        // pick 2 random students
        int student1 = GaGlobalRandomIntegerGenerator->Generate(numStudents-1);
        int student2 = GaGlobalRandomIntegerGenerator->Generate(numStudents-1);
        hash_map<Student*, int>::iterator it1;
        hash_map<Student*, int>::iterator it2;
        
        // Iterate through the hashmap _lookup, reducing student and increasing it1 until student == 0:
        //   now it points to a randomly chosen student.
        for (it1 = chromo->_lookup.begin();
             student1 != 0 && it1 != chromo->_lookup.end();
             it1++, student1--) ;
        
        // Same for it2
        for (it2 = chromo->_lookup.begin();
             student2 != 0 && it2 != chromo->_lookup.end();
             it2++, student2--) ;
        
        Student* firstStu = (*it1).first;
        Student* secondStu = (*it2).first;
        int slot1 = (*it1).second;
        int slot2 = (*it2).second;
        
        
        //Delete stu1 from first slot and insert stu2
        list<Student*>& theFirstSlot = chromo->_values[slot1];
        for (list<Student*>::iterator itFirst = theFirstSlot.begin(); itFirst != theFirstSlot.end(); itFirst++) {
            if ( *itFirst == firstStu )
            {
                theFirstSlot.erase(itFirst);
                theFirstSlot.push_back(secondStu);
                break;
            }
        }
        
        // Vice versa
        list<Student*>& theSecondSlot = chromo->_values[slot2];
        for (list<Student*>::iterator itSecond = theSecondSlot.begin(); itSecond != theSecondSlot.end(); itSecond++) {
            if ( *itSecond == secondStu )
            {
                theSecondSlot.erase(itSecond);
                theSecondSlot.push_back(firstStu);
                break;
            }
        }
        
        //update hashmap:
        chromo->_lookup[firstStu] = slot2;
        chromo->_lookup[secondStu] = slot1;
        
    }
    

    
}

float TTFitness::operator()(const GaChromosome* chromosome) const{
    const Chromosone* chromo = dynamic_cast<const Chromosone*>(chromosome);
    
    float score = 0;
    
    int numStudents = (int)chromo->_lookup.size();
    int numSlots = SLOTS_IN_DAY * Configuration::getInstance().numTutors();
    
    float maxscore = 6.1 * numStudents;
    
    // loop over all students
    for (hash_map<Student*,int>::const_iterator it = chromo->_lookup.begin(); it!=chromo->_lookup.end(); it++ ) {
        
        //is there overlapping?
        if ( !(chromo->_values[(*it).second].size()>1) ) score+=1.5;
            // Overlapping is particularly bad, so should merit a higher penalty than other lacking major requirements, eg not teaching the subject
        
        //Does the tutor teach the subject?
        //get tutor and time:
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
        bool canDoTut = true;
        list<int> tutNotSlots = tutor->getNotSlots();
        for (list<int>::iterator itTut = tutNotSlots.begin(); itTut != tutNotSlots.end(); itTut++)
        {
            if ( *itTut == (*it).second )
            {
                canDoTut = false;
                break;
            }
        }
        if (canDoTut) score++;
        
        //can the student do the time?
        bool canDoStu = true;
        list<int> stuNotSlots = (*it).first->getNotTimes();
        
        for (list<int>::iterator itStu = stuNotSlots.begin(); itStu != stuNotSlots.end(); itStu++)
        {
            if ( *itStu == time )
            {
                canDoStu = false;
                break;
            }
        }
        if (canDoStu) score++;
        
        //is this student already busy at this time?
        int engagements = 0;
        //loop over all tutors at this time
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
        
        
        
        
        // Are all the other appointments of this student in the same group?
        //    score for every appointment in the same group
        int samegroup = -1;
        // n.b. starts on -1 since we will find at least one student in this group: the one that we're currently iterating on!
        
        // Calculate start and end of times to search
        int loopStart, loopEnd;
        
        if (time < SLOTS_IN_DAY/3) loopStart = 0;
        else if (time < SLOTS_IN_DAY*2/3) loopStart = SLOTS_IN_DAY/3;
        else loopStart = SLOTS_IN_DAY*2/3;
        
        loopEnd = loopStart+(SLOTS_IN_DAY/3);
        
        // loop over all tutors:
        for (int tutorBase = 0; tutorBase < numSlots; tutorBase += SLOTS_IN_DAY) {
            // loop over the segment of times
            for (int loopTime = loopStart; loopTime < loopEnd; loopTime++) {
                int slot = tutorBase + loopTime;
                
                list<Student*> thisSlot = chromo->_values[slot];
                list<Student*>::iterator search;
                for (search = thisSlot.begin(); search != thisSlot.end(); search++) {
                    // If we find a student with the same baseID (ie the same person) in this same group:
                    if ( (*search)->getBaseID() == (*it).first->getBaseID() ) samegroup++;
                }
            }
        }
        score += samegroup * 0.5/6.0;
//         this score is so small because we must consider what happens when a student's / tutor's notTime conflicts with the grouping:
//             if we have four students grouped in the slots that the student can't do, we require that moving one student out of the group is profitable
//             the gain is +1 (since we're no longer breaching a notTime
//             the loss is x for each other slot in the group (3 in this example) + 3x for the slot being moved (since there were three others in the same group)
//             hence we require that 6x < 1. I chose 6x = 0.5.
        maxscore += ( (*it).first->getNoInterviews() - 1 ) * 0.5/6.0;
        
       
        
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
        if (pairings==1) score+=0.5;
        
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
    
    // MINOR:
    //  Is this the same solution that we found in the last run (if there was one)?
    //  Score for this is very low since we want all other requirements to take priority
    if (Configuration::getInstance().prevSolutionLoaded()) {
                
        //loop over all slots
        for (int i=0; i<numSlots; i++) {
            list<Student*> thisSlot = chromo->_values[i];
            list<int> prevSlot = Configuration::getInstance().getPrevSolution()[i];
            
            bool matching = true;
            if (thisSlot.empty()) {
                if (!prevSlot.empty()) matching = false;
            }
            else {
                
                // For each student in this slot,
                //   check if they're in the previous solution
                for (list<Student*>::iterator it = thisSlot.begin(); it!=thisSlot.end(); it++) {
                    // If this student is not present, set matching = false and break
                    if ( find(prevSlot.begin(), prevSlot.end(), (*it)->getBaseID()) == prevSlot.end() ) {
                        matching = false;
                        break;
                    }
                }
            }
            
            if (matching) score += 0.01;

        }
        maxscore += 0.01 * numSlots;
    }
    
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


