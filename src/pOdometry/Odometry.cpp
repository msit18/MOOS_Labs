/************************************************************/
/*    NAME: Michelle Sit                                    */
/*    ORGN: MIT                                             */
/*    FILE: Odometry.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Odometry.h"
#include <cmath>

using namespace std;

//---------------------------------------------------------
// Constructor

Odometry::Odometry()
{
    m_first_reading = false;
    m_current_x = 0;
    m_current_y = 0;
    m_previous_x = 0;
    m_previous_y = 0;
    m_total_distance = 0;
}

//---------------------------------------------------------
// Destructor

Odometry::~Odometry()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Odometry::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;

  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString();
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

    string key = msg.GetKey();
    double dval = msg.GetDouble();

    if (key == "NAV_X") {
      m_previous_x = m_current_x;
      m_current_x = msg.GetDouble();
    }
    else if (key == "NAV_Y") {
      m_previous_y = m_current_y;
      m_current_y = msg.GetDouble();
    }
   }

   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Odometry::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);

   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Odometry::Iterate()
{
  AppCastingMOOSApp::Iterate();

  double delta_x = m_current_x - m_previous_x;
  double delta_y = m_current_y - m_previous_y;

  if (m_current_x != m_previous_x || m_current_y != m_previous_y){
      m_total_distance += sqrt(delta_x*delta_x + delta_y*delta_y);
  }

  m_Comms.Notify("ODOMETRY_DIST", m_total_distance);

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Odometry::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);

  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;

    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
//      string param = tolower(biteStringX(line, '='));
//      string value = line;

      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);

      if(param == "foo") {
        //handled
      }
      else if(param == "bar") {
        //handled
      }
    }
  }

  RegisterVariables();
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Odometry::RegisterVariables()
{
  AppCastingMOOSApp::RegisterVariables();

  // Register("FOOBAR", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
}

//---------------------------------------------------------
// Procedure: BuildReport

bool Odometry::buildReport() {
  m_msgs << "Total distance: " << m_total_distance << endl;
  return true;
}
