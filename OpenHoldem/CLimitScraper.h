//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Interpreting the title-string 
//   and scraping limits, handnumber, etc. from the table
//
//******************************************************************************

#ifndef INC_CLIMITSCRAPER_H
#define INC_CLIMITSCRAPER_H

class CLimitScraper {
 public:
  CLimitScraper();
  ~CLimitScraper();
};

extern CLimitScraper *p_limit_scraper;

#endif INC_CLIMITSCRAPER_H