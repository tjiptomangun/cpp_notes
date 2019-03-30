/* Change to the SCHED_FIFO policy and the highest priority, then  */
/* lowest priority, then back to the original policy and priority. */

#include <unistd.h>
#include <sched.h>

#define CHECK(sts,msg)  \
  if (sts == -1) {      \
    perror(msg);        \
    exit(-1);           \
  }

main ()
{
  struct sched_param param;
  int my_pid = 0;
  int old_policy, old_priority;
  int sts;
  int low_priority, high_priority;

       /* Get parameters to use later.  Do this now  */
       /* Avoid overhead during time-critical phases.*/

  high_priority = sched_get_priority_max(SCHED_FIFO);
  CHECK(high_priority,"sched_get_priority_max");
  low_priority = sched_get_priority_min(SCHED_FIFO);
  CHECK(low_priority,"sched_get_priority_min");

       /* Save the old policy for when it is restored. */

  old_policy = sched_getscheduler(my_pid);
  CHECK(old_policy,"sched_getscheduler");

       /* Get all fields of the param structure.  This is where */
       /* fields other than priority get filled in.             */	

  sts = sched_getparam(my_pid, &param);
  CHECK(sts,"sched_getparam");

       /* Keep track of the old priority. */

  old_priority = param.sched_priority;

       /* Change to SCHED_FIFO, highest priority.  The param   */
       /* fields other than priority get used here.            */

  param.sched_priority = high_priority;
  sts = sched_setscheduler(my_pid, SCHED_FIFO, &param);
  CHECK(sts,"sched_setscheduler");

       /* Change to SCHED_FIFO, lowest priority.  The param */
       /* fields other than priority get used here, too.    */

  param.sched_priority = low_priority;
  sts = sched_setparam(my_pid, &param);
  CHECK(sts,"sched_setparam");

       /* Restore original policy, parameters.  Again, other  */
       /* param fields are used here.                         */

  param.sched_priority = old_priority;
  sts = sched_setscheduler(my_pid, old_policy, &param);
  CHECK(sts,"sched_setscheduler 2");

  exit(0);
}
