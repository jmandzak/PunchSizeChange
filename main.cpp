#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>

using namespace std;

//functions explained above definitions

double GetAvg(vector<double> data);

void PrintParts(vector<double> mins, vector<double> maxes, vector<double> avgs, ostream &os);

void PrintPartsGoodBad(vector<double> mins, vector<double> maxes, vector<double> avgs, double TOLmin, double TOLmax, ostream &os);

void PartChecker(vector<double> mins, vector<double> maxes, vector<double> avgs, double TOLmin, double TOLmax, ostream &os);

double PunchChange(vector<double> mins, vector<double> maxes, vector<double> avgs, double TOLmin, double TOLmax);

void PartsAfterPunch(vector<double> mins, vector<double> maxes, vector<double> avgs, double TOLmin, double TOLmax, double punchDifference, ostream &os);



int main() {
  double DOPmin;
  double DOPmax;
  double DOPavg;
  double TOLmin;
  double TOLmax;

  //these vectors will store the data points of each part
  vector<double> minimums;
  vector<double> maximums;
  vector<double> averages;

  string response;

  stringstream ss;

  //this introduces the user to the program
  cout << "Welcome! This program takes input on DOP size from you, the user,\nto give an accurate estimation on whether or not the punch size\nshould be changed.  Would you like to input data using a file or\nby directly inputting?\n\n(TYPE 'file' or 'input')\n\n";
  
  cin >> response;

  //this makes the response lowercase for easier comparison
  for(int i = 0; i < response.length(); i++)
  {
    response[i] = tolower(response[i]);
  }
  
  if(response == "input")
  {
    cout << "Please input MINIMUM tolerance:\n";
    cin >> TOLmin;
    cout << "Please input MAXIMUM tolerance:\n";
    cin >>TOLmax;

    cin.ignore();
    while(true)
    {
      cin.clear();
      cout << "At any time, type 'q' to quit\n\n";
      cout << "Please enter DOP min, DOP max, and DOP average\n";

      //I use getline to pull multiple data points at once, then put it
      //into a stringstream to parse the data into separate variables
      //which I then push_back into their respective vectors
      getline(cin, response);

      //cout << "response is " << response << '\n';
      if(response == "q")
      {
        break;
      }

      ss << response;
      ss >> DOPmin >> DOPmax >> DOPavg;
      ss.clear();

      minimums.push_back(DOPmin);
      maximums.push_back(DOPmax);
      averages.push_back(DOPavg);
    }
  }

  if(response == "file")
  {
    string file;
    string line;
    stringstream ss;

    ifstream fin;

    cout << "\nPlease enter the name of the file with the necessary data\n\n***NOTE*** Please make sure the data is listed in the following format:\n\nThe tolerances should be on a line by themselves, and each part should have it's own line\n\nMinimum Tolerance, Maximum Tolerance\nMinimum DOP, Maximum DOP, Average DOP\nMinumum DOP, Maximum DOP, etc.\n\nIF THE DATA IS NOT IN THIS FORMAT, RESULTS WILL BE SKEWED\n\n";

    cin >> file;

    fin.open(file.c_str());

    //standard file checker
    if(!fin.is_open())
    {
      cout << "File could not be opened\n";
      return -1;
    }

    //this first getline will grab the tolerances
    getline(fin, line);

    ss << line;
    ss >> TOLmin >> TOLmax;
    ss.clear();

    //this while loop will perform a similar function
    //of the while loop in the 'input' section
    while(getline(fin, line))
    {
      ss << line;
      ss >> DOPmin >> DOPmax >> DOPavg;
      ss.clear();

      minimums.push_back(DOPmin);
      maximums.push_back(DOPmax);
      averages.push_back(DOPavg);
    }
    fin.close();
  }

  else
  {
    cout << "Unrecognized command\n";
    return -2;
  }

  char answer;

  //I go ahead and assign the PunchChange value to a variable
  //so I can avoid a function call within the paramaters of the 
  //PartsAfterPunch function
  double change = PunchChange(minimums, maximums, averages, TOLmin, TOLmax);
  
  //this while loop is how the user will view the data generated from input
  while(answer != 'q')
  {
    cout << "\n\nType 'a' to see all parts entered\n";
    cout << "Type 'b' to see if the specs of all parts entered meet tolerance\n";
    cout << "Type 'c' to see how many good, undersized, and oversized parts were entered\n";
    cout << "Type 'd' to see recommended punch size change\n";
    cout << "Type 'e' to see parts after punch size change made\n";
    cout << "type 'f' to test a different punch size\n";
    cout << "Type 'q' to quit\n\n";

    cin >> answer;
  
    switch(answer)
    {
      case 'a':
        PrintParts(minimums, maximums, averages, cout);
        break;

      case 'b':
        PrintPartsGoodBad(minimums, maximums, averages, TOLmin, TOLmax, cout);
        break;

      case 'c':
        PartChecker(minimums, maximums, averages, TOLmin, TOLmax, cout);
        break;

      case 'd':
        cout << "\nRecommended punch size change is " << change << '\n';
        break;

      case 'e':
        PartsAfterPunch(minimums, maximums, averages, TOLmin, TOLmax, change, cout);
        break;

      case 'f':
        double userChange;
        cout << "What punch size change would you like to make?\n";
        cin >> userChange;
        PartsAfterPunch(minimums, maximums, averages, TOLmin, TOLmax, userChange, cout);
        break;

      case 'q':
        break;

      default:
        cout << "Unknown command.  Please enter a listed command\n";
    }
  }
  
  //put in a loop in case the user doesn't put 'y' or 'n'
  while(true)
  {
    cout << "Would you like to output the data to a file? (Y/N)\n";
    cin >> response;

    if(response == "Y" || response == "y")
    {
      //this is the only situation fout would be needed,
      //so I declared it here instead of up top to save memory
      ofstream fout;
      string fileName;

      cout << "Please enter a name for the file, ending with '.txt'\n";
      cin >> fileName;

      fout.open(fileName.c_str());
      
      //standard fout check for permissions
      if(!fout.is_open())
      {
        cout << "Could not write to file.  Would you like to try a different name? (Y/N)\n";
        cin >> response;
        
        if(response == "Y" || response == "y")
        {
          continue;
        }
        else if(response == "N" || response == "n")
        {
          break;
        }
        else
        {
          cout << "Unknown command.  Please enter 'Y' or 'N'\n";
          continue;
        }
      }

      //Here I call all the functions to print all the data to the file
      PrintParts(minimums, maximums, averages, fout);
      PrintPartsGoodBad(minimums, maximums, averages, TOLmin, TOLmax, fout);
      PartChecker(minimums, maximums, averages, TOLmin, TOLmax, fout);
      change = PunchChange(minimums, maximums, averages, TOLmin, TOLmax);
      fout << "\n\nRecommended Punch Change is " << change << "\n\n";
      PartsAfterPunch(minimums, maximums, averages, TOLmin, TOLmax, change, fout);

      cout << "File written\n";
      fout.close();
      break;
    }
    else if(response == "N" || response == "n")
    {
      break;
    }
    else
    {
      cout << "Unknown Command. Please answer with either 'Y' or 'N'\n";
      continue;
    }
  }
  
  return 0;
}

//*******END OF MAIN*******

//this function calculates the mean
double GetAvg(vector<double> data)
{
  double total = 0;
  for (int i = 0; i < data.size(); i++)
  {
    total += data[i];
  }
  double mean = total / data.size();
  return mean;
}

//this function prints the parts mins, avgs, and maxes by number
void PrintParts(vector<double> mins, vector<double> maxes, vector<double> avgs, ostream &os)
{
  os << "\n\n\n" << setw(20) << "MIN" << setw(20) << "AVG" << setw(20) << "MAX\n\n";
  for(int i = 0; i < mins.size(); i++)
  {
    os << setw(20) << mins[i] << setw(20) << avgs[i] << setw(20) << maxes[i] << "\n";
  }
}

//this function prints the parts mins,avgs, and maxes by whether or not they meet tolerance
void PrintPartsGoodBad(vector<double> mins, vector<double> maxes, vector<double> avgs, double TOLmin, double TOLmax, ostream &os)
{
  os << "\n\n\n" << setw(20) << "MIN" << setw(20) << "AVG" << setw(20) << "MAX\n\n";
  for(int i = 0; i < mins.size(); i++)
  {
    string DOPmin;
    string DOPmax;
    string DOPavg;

    //checks if min is in tolerance
    if(mins[i] <= TOLmax && mins[i] >= TOLmin)
    {
      DOPmin = "Good";
    }
    else
    {
      DOPmin = "Bad";
    }
    
    //checks if avg is in tolerance
    if(avgs[i] <= TOLmax && avgs[i] >= TOLmin)
    {
      DOPavg = "Good";
    }
    else
    {
      DOPavg = "Bad";
    }
    
    //checks if max is in tolerance
    if(maxes[i] <= TOLmax && maxes[i] >= TOLmin)
    {
      DOPmax = "Good";
    }
    else
    {
      DOPmax = "Bad";
    }

    os << setw(20) << DOPmin << setw(20) << DOPavg << setw(20) << DOPmax << "\n";
  }
}

//this function tells how many good, oversized, and undersized parts you have
void PartChecker(vector<double> mins, vector<double> maxes, vector<double> avgs, double TOLmin, double TOLmax, ostream &os)
{
  int undersizedParts = 0;
  int mostlyOversizedParts = 0;
  int oversizedParts = 0;
  int goodParts = 0;

  for(int i = 0; i < mins.size(); i++)
  {
    if(mins[i] < TOLmin)
    {
      undersizedParts++;
    }
    else if(avgs[i] > TOLmax)
    {
      oversizedParts++;
    }
    else if(maxes[i] > TOLmax)
    {
      mostlyOversizedParts++;
    }
    else
    {
      goodParts++;
    }
  }

  os << "\n\n***NOTE***\nOne part cannot be in multiple categories\nThe way parts are sorted is by order of importance\nThe order of importance is as follows:\nUndersized, Oversize AVERAGE, Oversize MAX, Good\n\n";
  os << "GOOD" << setw(20) << "UNDERSIZED" << setw(20) << "MAX IS OVERSIZED" << setw(20) << "AVG IS OVERSIZED\n";
  os << goodParts << setw(20) << undersizedParts << setw(20) << mostlyOversizedParts << setw(20) << oversizedParts << "\n\n";
}

//this function returns the recommended punch change
double PunchChange(vector<double> mins, vector<double> maxes, vector<double> avgs, double TOLmin, double TOLmax)
{
  double TOLwidth = TOLmax - TOLmin;
  double TOLavg = (TOLmax + TOLmin) / 2.0;
  
  double dopMinMean = GetAvg(mins);
  double dopMaxMean = GetAvg(maxes);
  double dopAvgMean = GetAvg(avgs);

  double realWidth = dopMaxMean - dopMinMean;

  if(realWidth > TOLwidth)
  {
    cout << "\n\n\n**********RECOMMENDED MACHINING CHANGE, VARIANCE OF PART EXCEEDS VARIANCE OF TOLERANCE**********\n\n\n";
  }

  double punchDifference = TOLavg - dopAvgMean;

  return punchDifference;
  //cout << "Recommended punch size change: " << punchDifference << '\n';
}

//this function shows what new parts would look like after punch change
void PartsAfterPunch(vector<double> mins, vector<double> maxes, vector<double> avgs, double TOLmin, double TOLmax, double punchDifference, ostream &os)
{
  //the strategy with this function is to create new vectors
  //so as to not modify the current vectors, then run
  //PartChecker with the new vectors
  vector<double> newMins;
  vector<double> newMaxes;
  vector<double> newAvgs;

  newMins.resize(mins.size());
  newMaxes.resize(maxes.size());
  newAvgs.resize(avgs.size());

  for(int i = 0; i < mins.size(); i++)
  {
    newMins[i] = mins[i] + punchDifference;
    newMaxes[i] = maxes[i] + punchDifference;
    newAvgs[i] = avgs[i] + punchDifference;
  }

  os << "\n\nParts After Punch Size Change\n\n";
  PrintParts(newMins, newMaxes, newAvgs, os);
  PrintPartsGoodBad(newMins, newMaxes, newAvgs, TOLmin, TOLmax, os);
  PartChecker(newMins, newMaxes, newAvgs, TOLmin, TOLmax, os);
}