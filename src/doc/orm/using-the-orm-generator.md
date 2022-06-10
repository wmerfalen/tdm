# ORM Generator Script

## Location: 
`src/ormgen`
If you don't have a symbolic link in your src folder, run `make ormgen`.


# Step 1: define your inputs
Copy the input.example file at `src/scripts/builder-generator/input.example`
and edit it to accomodate your needs. 

Example:
```
[start]

[std::string]
m_player_name
m_class

[player_id]
m_player_id

[initialize_row]
m_player_name
m_class

[end]
```

# Step 2: Call the script

If your table name is 'scores', and your input file is `my-input-file` run this:

```sh
./ormgen scores my-input-file
```

That's it! You'll have to make sure the sql is generated for the table, but that's
the only manual thing you'll have to do.
