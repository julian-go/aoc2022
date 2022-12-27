import os

year = 22;
template = "template.cpp"

year_string = "y" + str(year)
solution_folder = year_string.upper()
days = [f'day{i:02}' for i in range(1, 26)]

with open('header_template', 'r') as file :
  header_template = file.read()
  header_template = header_template.replace("_INC_GUARD_", "_" + year_string.upper() + "_H_")
  header_template = header_template.replace("_YEAR_", year_string)

#with open(year_string + ".h", 'w') as file:
#  file.write(header_template)

with open('solution_template', 'r') as file :
  solution_template = file.read()
  solution_template = solution_template.replace("_HEADER_", '"' + year_string + '.h"')
  solution_template = solution_template.replace("_YEAR_", year_string)

#os.mkdir(solution_folder)
for day in days:
    #os.mkdir(os.path.join(solution_folder, day))
    cppfile = os.path.join(solution_folder, day, year_string + day + ".cpp")
    with open(cppfile, 'w') as file:
        file.write(solution_template.replace("_DAY_", day))