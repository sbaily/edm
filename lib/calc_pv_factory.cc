// -*- c++ -*-
//
// calc_pv_factory.h
//
// kasemir@lanl.gov

#include<sys/time.h>
#include<unistd.h>   
#include<stdio.h>
#include<stdlib.h>
#include<float.h>
#include<math.h>
#include"postfix.h"
#include"alarm.h"
#include"calc_pv_factory.h"

static const char *whitespace = " \t\n\r";
#define CALC_PV_HUGE_VAL 1e50

#undef CALC_DEBUG
//#define CALC_DEBUG 1

// HashedExpression:
// One formula, hashed by name, read from the config file
// and converted into postfix notation.
// Can calc. result when fed arguments.
//
// CALC_PV_Factor:
// Called by main PV_Factory for "CALC\..." ProcessVariables.
// Reads the config file.
// When create("sum(fred, freddy)") is called,
// it looks for the formula "sum"
// and creates a CALC_ProcessVariable for sum with fred & freddy
// as arguments
//
// CALC_ProcessVariable
// Subscribes to arguments, recalculates value of formula
// whenever arguments change.
// If there are no arguments (-> no incoming events),
// timer is used to generate value events.

// ------------------------------------------------------------------
// HashedExpression ----------------------------------------------------
// ------------------------------------------------------------------
HashedExpression::HashedExpression()
{   name = 0; }

HashedExpression::HashedExpression(const char *_name, char *formula,
 char *rewriteString=NULL )
{
    short error;
    name = strdup(_name);
    if ( rewriteString ) {
      expStr.setRaw( rewriteString );
    }
    if (postfix(formula, this->compiled, &error) != 0)
    {
        fprintf(stderr, "CALC '%s': error in expression '%s'\n",
                name, formula);
        return;
    }
}

HashedExpression::~HashedExpression()
{
    if (name)
    {
        free(name);
        name = 0;
    }
}

bool HashedExpression::calc(const double args[], double &result)
{   return calcPerform((double *) args, &result, compiled) == 0; }

// Required for Hashtable<>:
size_t hash(const HashedExpression *item, size_t N)
{   return generic_string_hash(item->name, N); }

bool equals(const HashedExpression *lhs, const HashedExpression *rhs)
{   return strcmp(lhs->name, rhs->name)==0; }

// ------------------------------------------------------------------
// CALC_PV_Factory --------------------------------------------------
// ------------------------------------------------------------------
enum { HashTableSize = 43 };
typedef Hashtable<HashedExpression,
                  offsetof(HashedExpression, node),
                  HashTableSize> ExpressionHash;
static ExpressionHash *expressions;

static void checkForEmbedded (
  char *expression
) {

int l;
char *outer, inner[255+1];

  if ( !expression ) return;

  //printf( "checkForEmbedded - expression = [%s]\n", expression );

  l = strlen( expression );

  if ( l > 255 ) return;

  if ( l > 2 ) {

    if ( ( expression[0] == '{' ) && ( expression[l-1] == '}' ) ) {

      HashedExpression he;
      outer = strdup( expression );
      // 'he' will delete the strdup'ed  expression
      he.name = outer;
      ExpressionHash::iterator entry = expressions->find(&he);
      if (entry == expressions->end()) {

        strcpy( inner, &expression[1] );
        l = strlen( inner );
        inner[l-1] = 0;

        //printf( "New calc - name=%s, expression=%s\n",
        // expression, inner );

        expressions->insert( new HashedExpression( expression, inner ) );

      }

    }

  }

}

CALC_PV_Factory::CALC_PV_Factory()
{
#   ifdef CALC_DEBUG
    printf("CALC_PV_Factory created\n");
#   endif
    if (expressions)
    {
        fprintf(stderr, "Error: More than one CALC_PV_Factory created!\n");
        return;
    }
    
    expressions = new ExpressionHash();
    
    char *env = getenv(CALC_ENV);
    if (env)
    {
        env = strdup(env);
        char *start = env;
        while(start && *start)
        {
            char *end = strchr(start, ':');
            if (end)
                *end = '\0';
            parseFile(start);
            if (end)
                start = end+1;
            else
                start = 0;
        }
        free(env);
    }
    else
    {
        if (! parseFile(CALC_FILENAME))
        {
            const char *path=getenv("EDMFILES");
            if (path)
            {
                char *name = (char *) malloc(strlen(path)+
                                            strlen(CALC_FILENAME)+2);
                if (name)
                {
                    sprintf(name, "%s/%s", path, CALC_FILENAME);
                    parseFile(name);
                    free(name);
                }
            }
        }
    }
}

CALC_PV_Factory::~CALC_PV_Factory()
{
    if (expressions)
    {
        ExpressionHash::iterator i;
        i=expressions->begin();
        while (i!=expressions->end())
        {
            HashedExpression *e = *i;
            expressions->erase(i);
            delete e;
            i=expressions->begin();
        }
        delete expressions;
        expressions = 0;
    }
#   ifdef CALC_DEBUG
    printf("CALC_PV_Factory deleted\n");
#   endif
}

bool CALC_PV_Factory::parseFile(const char *filename)
{
    char line[200], name[200], newArgList[200];
    size_t len;

#   ifdef CALC_DEBUG
    printf("CALC_PV_Factory::parseFile '%s'\n", filename);
#   endif
    FILE *f = fopen(filename, "rt");
    if (!f)
    {
#       ifdef CALC_DEBUG
        printf("CALC_PV_Factory::parseFile failed\n");
#       endif
        return false;
    }

    // Check version code
    if (!fgets(line, sizeof line, f)  ||
        strncmp(line, "CALC1", 5))
    {
        fprintf(stderr, "Invalid CALC configuration file '%s'\n",
                filename);
        fclose(f);
        return false;
    }

    // Loop over lines
    bool need_name = true;
    char *p;
    while (fgets(line, sizeof line, f))
    {
        len = strlen(line);
        if (len <= 0 || (len > 0 && line[0] == '#')) // skip comments
            continue;
        // Remove trailing white space
        while (len>0 && strchr(whitespace, line[len-1]))
        {
            line[len-1] = '\0';
            --len;
        }
        if (len <= 0)
            continue;
        // Skip leading white space
        p=line;
        while (strchr(whitespace, *p))
        {
            ++p;
            --len;
        }
        
        if (need_name)
        {
            strcpy(name, p);
            strcpy( newArgList, "" );
            need_name = false;
        }
        else
        {

	  if ( p[0] == '@' ) {

            strcpy( newArgList, &p[1] );

	  }
	  else {

            need_name = true;
#           ifdef CALC_DEBUG
            printf("CALC_PV_Factory::parseFile expr '%s' <-> '%s'\n",
                   name, p);
#           endif

	    if ( strcmp( newArgList, "" ) ) {

              expressions->insert(new HashedExpression(name, p,
               newArgList));

	    }
	    else {

              expressions->insert(new HashedExpression(name, p));

	    }

	  }

        }
    }
    fclose(f);
    return true;
}

// Fills *arg with copy of next argument found,
// returns 0 or updated position p
static const char *get_arg(const char *p, char **arg)
{
    // find start...
    while (*p && strchr(", \t)", *p))
        ++p;
    if (! *p)
        return 0;

    // find end, including spaces...
    const char *end = p+1;
    int braces = 0;
    while (*end)
    {
        if (*end == '(')
            ++braces;
        else if (*end == ')')
        {
            --braces;
            if (braces < 0)
                break;
        }
        else if (*end == ',')
        {
            if (braces==0)
                break;
        }
        ++end;
    }
    // end is on character NOT to copy: '\0'  ','  ')'
    // remove trailing space
    while (end > p  && strchr(whitespace, *(end-1)))
        --end;
    // copy
    int len = end - p;
    if (len <= 0)
        return 0;
    // Compiler created trash when using *arg instead of temp. narg
    char *narg = (char *)malloc(len+1);
    memcpy(narg, p, len);
    narg[len] = '\0';
    *arg = narg;

    return end;
}

ProcessVariable *CALC_PV_Factory::create(const char *PV_name)
{
    char *arg_name[CALC_ProcessVariable::MaxArgs];
    char *exp_arg_name[CALC_ProcessVariable::MaxArgs];
    size_t i, ii, l, arg_count = 0;
    const char *p;
    char *p1, *start;
    int more, findingInline = 0, gettingFirst;
    CALC_ProcessVariable *pv;

    for (i=0; i<CALC_ProcessVariable::MaxArgs; ++i)
        arg_name[i] = 0;
    
    // Locate expression: start...
    while (strchr(whitespace, *PV_name))
        ++PV_name;
    p = PV_name;
    // end...
    more = 1;
    //printf( "*p = [%s]\n", p );
    if ( *p == '(' ) more = 0;
    while (*p && more) {
        if ( *p == '{' ) findingInline++;
        if ( ( *p == '}' ) && findingInline ) findingInline--;
        ++p;
        if ( ( *p == '(' ) && !findingInline ) more = 0;
	//printf( "%c, findingInline = %-d, more = %-d\n",
        // (char) *p, findingInline, more );
    }
    // copy
    int len = p - PV_name; 
    if (len <= 0)
    {
        fprintf(stderr, "Empty expression '%s'\n", PV_name);
        return 0;
    }
    char *expression = (char *)malloc(len+1);
    memcpy(expression, PV_name, len);
    expression[len] = '\0';
    
    while (*p && strchr(whitespace, *p))
        ++p;
    // Do arguments follow?
    if (*p == '(')
    {
        ++p;
        while ((p=get_arg(p, &arg_name[arg_count])) != 0)
            ++arg_count;
    }

#   ifdef CALC_DEBUG
    printf("CALC_PV_Factory::create: '%s'\n", PV_name);
    printf("\texpression: '%s'\n", expression);
    for (i=0; i<arg_count; ++i)
        printf("\targ %d: '%s'\n", i, arg_name[i]);
#   endif    
    checkForEmbedded( expression );
    HashedExpression he;
    he.name = expression;
    ExpressionHash::iterator entry = expressions->find(&he);
    if (entry == expressions->end())
    {
        fprintf(stderr, "Unknown CALC expression '%s'\n", expression);
        return 0;
    }

    if ( strcmp( (*entry)->expStr.getRaw(), "" ) ) {

      char **sym;
      char **val;
      sym = (char **) new char*[arg_count];
      val = (char **) new char*[arg_count];
      for ( i=0; i<arg_count; ++i ) {
        sym[i] = strdup( " " );
        sym[i][0] = (char) i+65;
        val[i] = strdup( arg_name[i] );
      }
      (*entry)->expStr.expand1st( arg_count, sym, val );
      free( sym );
      free( val );

      for ( i=0; i<CALC_ProcessVariable::MaxArgs; ++i )
        exp_arg_name[i] = 0;

      i = ii = l = 0;
      p1 = (*entry)->expStr.getExpanded();
      gettingFirst = 1;

      while ( ii < strlen( p1 ) ) {

        if ( gettingFirst ) {

          if ( p1[ii] != ' ' ) {
            start = &p1[ii];
	    l = 1;
            gettingFirst = 0;
	  }

	}
	else {

          if ( p1[ii] == ',' ) {
            exp_arg_name[i] = (char *) malloc(l+1);
            strncpy( exp_arg_name[i], start, l );
            exp_arg_name[i][l] = 0;
	    gettingFirst = 1;
	    i++;
	    l = 0;
	  }
	  else {
	    l++;
	  }

	}

	ii++;

      }

      if ( !gettingFirst ) {

            exp_arg_name[i] = (char *) malloc(l+1);
            strncpy( exp_arg_name[i], start, l );
            exp_arg_name[i][l] = 0;
	    i++;

      }

#   ifdef CALC_DEBUG
      printf( "got %-d exp args\n", i );
      for ( ii=0; ii<i; ii++ ) {
	printf( "[%s]\n", exp_arg_name[ii] );
      }

      if ( strcmp( (*entry)->expStr.getRaw(), "" ) ) {
        printf( "raw: [%s]\n", (*entry)->expStr.getRaw() );
      }

      if ( strcmp( (*entry)->expStr.getExpanded(), "" ) ) {
        printf( "exp: [%s]\n", (*entry)->expStr.getExpanded() );
      }
#   endif    

      pv = new CALC_ProcessVariable(PV_name, *entry,
       i, (const char **)exp_arg_name);

      for ( ii=0; ii<i; ii++ ) {
	free( exp_arg_name[ii] );
        exp_arg_name[ii] = NULL;
      }

      pv->value = 0.0;

    }
    else {

      pv = new CALC_ProcessVariable(PV_name, *entry,
       arg_count, (const char **)arg_name);

      pv->value = 0.0;

    }

    // 'he' will delete the allocated expression
    for (i=0; i<arg_count; ++i) {
      free( arg_name[i] );
      arg_name[i] = NULL;
    }

    return pv;

}

// ------------------------------------------------------------------
// CALC_ProcessVariable ---------------------------------------------
// ------------------------------------------------------------------

CALC_ProcessVariable::CALC_ProcessVariable(const char *name,
                                           HashedExpression *_expression,
                                           size_t _arg_count,
                                           const char *arg_name[])
        : ProcessVariable(name)
{
    size_t i;

#   ifdef CALC_DEBUG
    printf("CALC_ProcessVariable '%s'\n", name);
#   endif
    
    for (i=0; i<MaxArgs; ++i)
    {
        arg[i] = 0.0;
        arg_pv[i] = 0;
    }

    precision = 4;
    upper_display = 10.0;
    lower_display = 0.0;
    upper_alarm = DBL_MAX;
    lower_alarm = -DBL_MAX;
    upper_warning = DBL_MAX;
    lower_warning = -DBL_MAX;
    upper_ctrl = 10.0;
    lower_ctrl = 0.0;
    
    expression = _expression;
    arg_count = _arg_count;

    // first go through and populate args and create pvs
    for (i=0; i<arg_count; ++i)
    {
        // Is this argument a number or another PV?
        // Poor excuse for a real "number" check:
        if (strchr("0123456789+-.", arg_name[i][0]) &&
            strspn(arg_name[i], "0123456789+-.eE"))
        {   // It's a number
            arg[i] = strtod(arg_name[i], 0);
            //if (arg[i] == HUGE_VAL || arg[i] == -HUGE_VAL)
            if (arg[i] == CALC_PV_HUGE_VAL || arg[i] == -CALC_PV_HUGE_VAL)
            {
                fprintf(stderr, "CALC PV %s: invalid number arg '%s'\n",
                        name, arg_name[i]);
                arg[i] = 0.0;
            }
#   ifdef CALC_DEBUG
            printf("arg[%d] = CONSTANT %g\n", i, arg[i]);
#   endif
        }
        else
        {   // It's a PV
            arg_pv[i] = the_PV_Factory->create(arg_name[i]);
#   ifdef CALC_DEBUG
            printf("arg[%d] = PV %s\n", i, arg_name[i]);
#   endif
        }
    }

    // then add callbacks
    for (i=0; i<arg_count; ++i)
    {
        if (arg_pv[i])
        {
            arg_pv[i]->add_conn_state_callback(status_callback, this);
            arg_pv[i]->add_value_callback(value_callback, this);
        }
    }

}

CALC_ProcessVariable::~CALC_ProcessVariable()
{
#ifdef CALC_DEBUG
    printf("CALC_ProcessVariable(%s) deleted\n", get_name());
#endif
    for (size_t i=0; i<arg_count; ++i)
    {
        if (arg_pv[i])
        {
            arg_pv[i]->remove_value_callback(value_callback, this);
            arg_pv[i]->remove_conn_state_callback(status_callback, this);
            arg_pv[i]->release();
            arg_pv[i] = 0;
        }
    }
}

void CALC_ProcessVariable::status_callback(ProcessVariable *pv, void *userarg)
{
    CALC_ProcessVariable *me = (CALC_ProcessVariable *)userarg;
#   ifdef CALC_DEBUG
    printf("CALC %s: status callback from %s.\n",
           me->get_name(), pv->get_name());
#   endif
    me->recalc();
    me->do_conn_state_callbacks();
}

void CALC_ProcessVariable::value_callback(ProcessVariable *pv, void *userarg)
{
    CALC_ProcessVariable *me = (CALC_ProcessVariable *)userarg;
#   ifdef CALC_DEBUG
    printf("CALC %s: value callback %s\n",
           me->get_name(), pv->get_name());
#   endif
    if ( me->is_valid() ) {
      me->recalc();
      me->do_value_callbacks();
    }
}

void CALC_ProcessVariable::recalc()
{
    size_t i;
    status = 0;
    severity = 0;
    time = 0;
    nano = 0;
    // Evaluate arguments, if any:
    for (i=0; i<arg_count; ++i)
    {
        if (!arg_pv[i])
        {
#   ifdef CALC_DEBUG
            printf("arg[%d] = constant %g\n", i, arg[i]);
#   endif
            continue;
        }
        if (arg_pv[i]->is_valid())
        {
            arg[i] = arg_pv[i]->get_double();
            // Maximize time stamp and severity
            if (arg_pv[i]->get_time_t() > time  ||
                (arg_pv[i]->get_time_t() == time  ||
                 arg_pv[i]->get_nano() > nano))
            {
                time = arg_pv[i]->get_time_t();
                nano = arg_pv[i]->get_nano();
            }
            if (arg_pv[i]->get_severity() > severity)
            {
                severity = arg_pv[i]->get_severity();
                status   = arg_pv[i]->get_status();
            }
#           ifdef CALC_DEBUG
            printf("arg[%d] = PV %s %g stat %d sevr %d\n",
                   i, arg_pv[i]->get_name(), arg[i],
                   arg_pv[i]->get_status(), arg_pv[i]->get_severity());
#           endif
        }
        else
        {
            status = UDF_ALARM;
            severity = INVALID_ALARM;
#           ifdef CALC_DEBUG
            printf("arg[%d] = PV %s is invalid\n",
                   i, arg_pv[i]->get_name());
#           endif
        }
    }
        
    if (severity != INVALID_ALARM)
        expression->calc(arg, value);

    if (time == 0)
    {
        struct timeval t;
        gettimeofday(&t, 0);
        time = t.tv_sec;
        nano = t.tv_usec*(unsigned long)1000;
    }

#   ifdef CALC_DEBUG
    printf("Result: %g stat %d sevr %d (%s)\n",
           value, status, severity,
           (is_valid() ? "valid" : "invalid"));
#   endif
}

bool CALC_ProcessVariable::is_valid() const
{
    size_t i;
    for (i=0; i<arg_count; ++i)
    {
        // arg_pv == 0 -> have constant value,
        // otherwise the PV needs to be valid
        if (arg_pv[i]!=0 && !arg_pv[i]->is_valid())
            return false;
    }
    return true;
}

static ProcessVariable::Type calc_type =
{
    ProcessVariable::Type::real,
    64,
    "real:64"
};

static ProcessVariable::specificType calc_specific_type =
{
    ProcessVariable::specificType::real,
    64
};

const ProcessVariable::Type &CALC_ProcessVariable::get_type() const
{   return calc_type; }

const ProcessVariable::specificType &CALC_ProcessVariable::get_specific_type() const
{   return calc_specific_type; }

double CALC_ProcessVariable::get_double() const
{   return value; }

size_t CALC_ProcessVariable::get_dimension() const
{   return 1; }

const char *CALC_ProcessVariable::get_char_array() const
{   return 0; }

const int *CALC_ProcessVariable::get_int_array() const
{   return 0; }

const double *CALC_ProcessVariable::get_double_array() const
{   return &value; }

time_t CALC_ProcessVariable::get_time_t() const
{   return time; }

unsigned long CALC_ProcessVariable::get_nano() const
{   return nano; }

short CALC_ProcessVariable::get_status() const
{   return status; }

short CALC_ProcessVariable::get_severity() const
{   return severity; }

short CALC_ProcessVariable:: get_precision() const
{   return precision; }

double CALC_ProcessVariable::get_upper_disp_limit() const
{   return upper_display; }

double CALC_ProcessVariable::get_lower_disp_limit() const
{   return lower_display; }

double CALC_ProcessVariable::get_upper_alarm_limit() const
{   return upper_alarm; }

double CALC_ProcessVariable::get_upper_warning_limit() const
{   return upper_warning; }

double CALC_ProcessVariable::get_lower_warning_limit() const
{   return lower_warning; }

double CALC_ProcessVariable::get_lower_alarm_limit() const
{   return lower_alarm; }

double CALC_ProcessVariable::get_upper_ctrl_limit() const
{   return upper_ctrl; }

double CALC_ProcessVariable::get_lower_ctrl_limit() const
{   return lower_ctrl; }

bool CALC_ProcessVariable::put(double value)
{   return false; }

bool CALC_ProcessVariable::put(const char *value)
{   return false; }

bool CALC_ProcessVariable::put(int value)
{   return false; }

bool CALC_ProcessVariable::putText(char *value)
{   return false; }

bool CALC_ProcessVariable::putArrayText(char *value)
{   return false; }