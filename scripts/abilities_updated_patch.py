def check_ability_exists(abilities, target_ability):
    for ability in abilities:
        temp = ability.split(' ')
        if ((temp[1] == target_ability) and (temp[1] != "ABILITY_NONE\n")):
            return temp[0]
    return -1


# constants
hidden_ability_index = 0x1A
base_stats_size = 0x1C
base_stats = 0x08254784 + hidden_ability_index
species_max = 412
to_write = "// patch abilities\n\n"
firered_abilities = []

# populate list of abilities
with open('./../ability_list', 'r') as f:
    firered_abilities = f.readlines()
    f.close()

with open('./../hiddenlist', 'r+') as f:
    hidden_abilities = f.readlines()
    for ability in hidden_abilities:
        temp = ability.split(' ')
        value = check_ability_exists(firered_abilities, temp[1])
        # if hidden ability exists in generation 3 abilities list
        # and species less than species max, append
        if ((value > 0) and (int(temp[0]) < species_max)):
            to_write += ("\t\t.org " + hex((int(temp[0])*base_stats_size) + base_stats).upper().replace('X', 'x') + "\n" +
                "\t\t.byte " + hex(int(value)).upper().replace('X','x') + "// " + temp[1] + "\n")

with open('./../patches/hidden_abilities/hiddenability.s', 'a+') as f:
    f.write('\n' + to_write)
