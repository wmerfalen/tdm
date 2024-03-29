<?php require_once(dirname(__FILE__) . '/init.php'); \siege\start('2018-11-07','Dynamics First, Optimizations Second.'); ?>
<p class='content'>
<b>It&apos;s easy</b> to fall into the trap of <i>endless optimization</i>.
Especially when tasked with upgrading a codebase that has been hacked on by
several coders of varying skillsets and expertise. During the development of 
<a class='project'>cm++, I&apos;ve faced many tempting invitations to speed up the 
codebase. The thrill of squeezing every last bit of performance out of an already
beefy machine is tantalizing, but at the same time a <b>literal trap</b>. 
64bit machines these days can handle a very hefty workload.. and when
you&apos;re entire game is text-based, you really can't complain about speed. I've seen
MUDs using legacy versions of CircleMUD with over a dozen players on at any give time.
There really isn't a bottleneck unless someone is purposely trying to create one. But
have you seen the data structures that CircleMUD employs? The codebase is entirely in C,
so that means manual allocations and deallocations, but most importantly... it means that
nearly <b>everything</b> that is at the root of the Circle core is a dreaded <i>Linked List</i>.

Now what do I mean when I say <blockquote class='horror'><u>Dreaded</u> Linked List</blockquote>?
I'm more talking about the utter waste of cache efficiency that you don't get for free by the processor.
I'm not a cache expert by any means, but there is one thing I know, and it's this:
<b class='philosophy'>The array is faster than the linked list</b>. A Linked List is a great way
to grow or shrink a segment of data, especially when you don't have optimized containers already
at your disposal like with a language like C++. Sure, there are container libs out there, I know that.
But that's neither here nor there, because CircleMUD was made almost entirely out of Linked List structures.
The CircleMUD game_loop is riddled with for loops that iterate over *every* single item within
a linked list. Mind you, this is a single-threaded sleep-based infinite loop. So, you're not going to
see any worker threads latching onto this data and thus divying out the workload. No, instead,
you've got a Linked List of all <b>players and mobs</b> stored in a linked list variable named <b>character_list</b>.
The gameloop will iterate through all characters one by one. If the current character needs to
have it's socket input handled, the gameloop will branch out and do everything necessary to complete
this transaction. Once that player is done, we move on to the next. Now as far as cache efficiency goes,
this isn't a very good practice-- <b><u>But that's fine</u></b>: CircleMUD has been able to get 
away with using Linked Lists for several decades now. So when I spend a few months replacing all
Linked Lists in cm++ with that of an STL container that guarantees contiguous memory, I tend to 
look back and think of all the <b>game dynamics</b> coding I could have been doing. 

This existential crisis that you feel when you suddenly realize that you've spent more hours 
trying to optimize away the inefficencies of a codebase than you have making the codebase
a unique and fun experience, is the very reason why I'm taking this new altered approach 
to cm++ development. As soon as the core is at an alpha level of stability, I'll waste no
time in creating a sandboxed playground for builders to easily create awesome game dynamics.
Linked list? Sure, that's fine, whatever.. I can't focus on that. I need to focus on what
will make the game fun, and not so much what will make the game run fast. Remember, a fast
game is nothing if nobody likes it. 
	<?php echo Page::get()->signature(); ?>
</p>
<?php
\siege\end();
