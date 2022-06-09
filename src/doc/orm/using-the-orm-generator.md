# ORM Generator Script

## Location: 
`src/scripts/builder-generator/gen.sh`

# Step 1: define your inputs
Edit the file `src/scripts/builder-generator/input`

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

For an example, see `src/scripts/builder-generator/input.example`

# Step 2: Call gen.sh

If your table name is 'scores', run this:
```sh
./scripts/builder-generator/gen.sh scores
```

That's it! You'll have to make sure the sql is generated for the table, but that's
the only manual thing you'll have to do.
