# Player accumulators

## The general idea
Player accumulators are data sets that belong to a table and are tied to a player's `db_id`.
We use a specific value (or values) to accumulate as the player interacts with the world.
Right now, the best example of this is the `notch` orm object.

## The notch ORM object
The notch orm object is a classic accumulator strategy. It keeps track of a player and
an achievement name. Along with that, we also increment the points associated with said
achievement. 

If we could abstract this pattern away, it would make database logic less painful to use.


# The problem
Currently, the only way to create a new orm object is to copy and paste parts of another
orm object. This can be error-prone and can cause atleast a couple of hours of debugging.

# The ideal solution
Simply put, I'd like to define the members of an orm object, and let boost preprocessor
take care of the rest.
