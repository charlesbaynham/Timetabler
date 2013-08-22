//
//  pdfOutput.cpp
//  Timetabler
//
//  Created by Charles Baynham on 21/08/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "pdfOutput.h"

using namespace std;

ReportResource::ReportResource(finishedTT timetable, Wt::WObject *parent)
: _timetable(timetable), Wt::WResource(parent)
{
    suggestFileName("report.pdf");
}

void ReportResource::handleRequest(const Wt::Http::Request& request,
                               Wt::Http::Response& response)
{
    response.setMimeType("application/pdf");
    
    HPDF_Doc pdf = HPDF_New(error_handler, 0);
    
    // Note: UTF-8 encoding (for TrueType fonts) is only available since libharu 2.3.0 !
    HPDF_UseUTFEncodings(pdf);
    
    renderReport(pdf);
    
    HPDF_SaveToStream(pdf);
    unsigned int size = HPDF_GetStreamSize(pdf);
    HPDF_BYTE *buf = new HPDF_BYTE[size];
    HPDF_ReadFromStream (pdf, buf, &size);
    HPDF_Free(pdf);
    response.out().write((char*)buf, size);
    delete[] buf;
}


void ReportResource::renderReport(HPDF_Doc pdf) {
    
    // Do the students' view:
    
    // Headers
    string html("<h1>Student View</h1>"
                "<table border='1'>"
                "<tr>"
                "<th>Student</th>\n");
    
    for (int i=1; i<=SLOTS_IN_DAY; i++) {
        html += "<th>Slot " + to_string(i) + "</th>\n";
    }
    
    html += "</tr>\n";
    
    // get student timetable
    studentTTList studentView = _timetable.getStudentOrientated()->getTT();
    
    // loop over all students
    
    for (studentTTList::iterator it = studentView.begin(); it != studentView.end(); it++) {
        
        html += "<tr>\n<td>" + (*it).second->getStudentName() + "</td>\n";
        
        //loop over times
        for (int time = 0; time < SLOTS_IN_DAY; time++) {
            html += "<td>" + (*it).second->getTutorName(time) + "</td>\n";
        }
        
        html += "</tr>\n";
    }
    
    html +=     "</table>";
    
    
    // --------------------
    
    html += "<h1>Tutor View</h1>"
                "<table border='1'>"
                "<tr>"
                "<th>Tutor</th>\n";
    
    for (int i=1; i<=SLOTS_IN_DAY; i++) {
        html += "<th>Slot " + to_string(i) + "</th>\n";
    }
    
    html += "</tr>\n";
    
    // get tutor timetable
    tutorTTList tutorView = _timetable.getTutorOrientated()->getTT();
    
    // loop over all tutors
    
    for (tutorTTList::iterator it = tutorView.begin(); it != tutorView.end(); it++) {
        
        html += "<tr>\n<td>" + (*it)->getTutorName() + "</td>\n";
        
        //loop over times
        for (int time = 0; time < SLOTS_IN_DAY; time++) {
            
            string data;
            Student* student = (*it)->getStudent(time);
            
            data += (student == NULL ? "-" : student->getName() + " (" + student->getSubject()->getName() + ")");
            
            html += "<td>" + data + "</td>\n";
        }
        
        html += "</tr>\n";
    }
    
    html +=     "</table>";
//    "<style>"
//    "table td {"
//    "   white-space: normal;"
//    "}"
//    "</style>";
    
    
    
    renderPdf(html, pdf);
}

void ReportResource::renderPdf(const Wt::WString& html, HPDF_Doc pdf)
{
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_LANDSCAPE);
    
    Wt::Render::WPdfRenderer renderer(pdf, page);
    renderer.setMargin(0.54);
    renderer.setDpi(200);
    renderer.render(html);
}

