//
//  pdfOutput.h
//  Timetabler
//
//  Created by Charles Baynham on 21/08/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#ifndef __Timetabler__pdfOutput__
#define __Timetabler__pdfOutput__

#include <iostream>
#include <Wt/WPushButton>
#include <Wt/WResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include <Wt/Render/WPdfRenderer>
#include <Wt/WApplication>

#include "constants.h"
#include "output.h"

#include "hpdf.h"

namespace {
    void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no,
                       void *user_data) {
        fprintf(stderr, "libharu error: error_no=%04X, detail_no=%d\n",
                (unsigned int) error_no, (int) detail_no);
    }
}

class ReportResource : public Wt::WResource
{
public:
    ReportResource(finishedTT timetable, Wt::WObject *parent = 0);
    
    virtual void handleRequest(const Wt::Http::Request& request,
                               Wt::Http::Response& response);
    
private:
    void renderReport(HPDF_Doc pdf);
    
    void renderPdf(const Wt::WString& html, HPDF_Doc pdf);
    
    finishedTT _timetable;
};

#endif /* defined(__Timetabler__pdfOutput__) */
