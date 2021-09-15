# Listing Uniques
` js list_op()`

# Instantiating a Unique
` js ira('nickname') `

# Example
- Instantiating the 'DXC DemonLance XIV' (base object PSG-1 sniper rifle)
`
65000H 6500M 65000V > js ira('dlance');
str: psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}
running: "instantiate_rifle_attachment psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}"
[+] Done
[+] It is done.
instantiated one for you

65000H 6500M 65000V > 

65000H 6500M 65000V > i
You are carrying:
(2) PSG-1 Sniper Rifle


`
