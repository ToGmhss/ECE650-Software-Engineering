#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import re


def pp(x):
    """Returns a pretty-print string representation of a number.
       A float number is represented by an integer, if it is whole,
       and up to two decimal places if it isn't
    """
    if isinstance(x, float):
        if x.is_integer():
            return str(int(x))
        else:
            return "{0:.2f}".format(x)
    return str(x)

class point(object):
    """A point in a two dimensional space"""
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    # align with (n,m)
    def __repr__(self):
        return '(' + pp(self.x) + ',' + pp(self.y) + ')' 

    # override for identifying points - fix bug in gg
    def __eq__(self, other):
        if not isinstance(other, point):
            # 不与非 Point 对象比较
            return NotImplemented
        return self.x == other.x and self.y == other.y

    # solves: unhashable type: 'Point
    def __hash__(self):
        return hash((self.x, self.y))

class line(object):
    """A line between two points"""
    def __init__(self, src, dst):
        self.src = src
        self.dst = dst

    def __repr__(self):
        return '['+ str(self.src) + '-->' + str(self.dst) + ']'


def intersect(l1, l2):
    """Returns a point at which two lines intersect"""
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y

    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))

    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)

    # fix bug for 0 denominator
    # 平行-no inter 或 重叠-infinite inter
    if xden == 0 or yden == 0:
        # 明确没有重叠的街道，只考虑一个交点在一起
        a, b = l1.src, l1.dst # a not = b
        c, d = l2.src, l2.dst
        if a == c or a == d:
            return a
        if b == c or b == d:
            return b
        return None

    xcoor =  xnum / xden
    ycoor = ynum / yden

    # add = for corner cases
    line1_x_cond = min(x1, x2) <= xcoor <= max(x1, x2)
    line1_y_cond = min(y1, y2) <= ycoor <= max(y1, y2)
    line2_x_cond = min(x3, x4) <= xcoor <= max(x3, x4)
    line2_y_cond = min(y3, y4) <= ycoor <= max(y3, y4)

    if line1_x_cond and line1_y_cond and line2_x_cond and line2_y_cond:
        return point(xcoor, ycoor)
    return None


'''
Definition:
    1. V:
        The set V is defined so that there is exactly one vertex corresponding to: 
        (a) each intersection,
        and, 
        (b) to each end-point of a line segment of a street that intersects with another street.
    2. E:
        The set E is defined so that there is an edge between two vertices iff: 
        (a) at least one of them is an intersection, 
        (b) both lie on the same street, and, 
        (c) one is reachable from the other without
        traversing another vertex. 
'''
class graph():
    def __init__(self):
        self.streets = {} # {st_name: (coord_list, line_list),...}
        ''' intersections
        {
            (st1, st2): {
                (line1, line2): inter1, ...
                (line3, line4): inter2
            }, ...
        }
        '''
        self.intersections = {} 
        
        '''
        {st1: {line1: [inter1, ...], ...}, ...}
        '''
        self.vertices = {} 
        self.edges = []
        #self.st_seg_v = {}


    # for debug only
    def check_status(self):
        print("streets: ", self.streets, "\n")
        print("inters:", self.intersections, "\n")
        print("vertices", self.vertices, "\n")


    def street_exist(self, street):
        if street in self.streets:
            return True
        return False


    def append_ver(self, st, line, inter):
        # st的第一个line的交点
        if st not in self.vertices:
            self.vertices[st] = {line: [inter]}
        else:
            # st里同一个line的新的交点
            if line in self.vertices[st]:
                # 确保是新的交点
                if inter not in self.vertices[st][line]:
                    self.vertices[st][line].append(inter)
            # st里新line的交点
            else:
                self.vertices[st][line] = [inter]


    '''st1, st2 are st names'''
    def new_st_intersection(self, st1, st2):
        st1_lines = self.streets[st1][1]
        st2_lines = self.streets[st2][1] # bug fix
        for line1 in st1_lines:
            for line2 in st2_lines:
                inter = intersect(line1, line2)
                '''print'''
                # print("cal new inter:")
                # print(f"st1: {st1}, st2: {st2}")
                # print(f"l1: {line1}, l2: {line2}")
                # print(f"inter: {inter}")
                if not inter:
                    continue
                # intersections
                '''{
                    (st1, st2): {(line1, line2): inter1, ...}, ...
                }
                '''
                new_key = (st1, st2)
                line_pair = (line1, line2)
                # self.intersections[new_key] doesn't exist
                if new_key not in self.intersections:
                    self.intersections[new_key] = {line_pair: inter}
                else:
                    # self.intersections[new_key][line_pair] dosen't exist
                    self.intersections[new_key][line_pair] = inter

                # vertices
                '''
                {st1: {line1: [inter1, ...], ...}, ...}
                '''
                self.append_ver(st1, line1, inter)
                self.append_ver(st2, line2, inter)
                
                
    def del_st_inter(self, st):
        # del inters, vertices are deleted as well
        st_pair_to_del = [key for key in self.intersections if st in key]
        '''print'''
        # print(f"to del: {st}")
        # print(st_pair_to_del)
        for st_pair in st_pair_to_del: # key: (st, other_st)
            '''todo: del self.vertices'''
            dic = self.intersections[st_pair] # {(line1, line2): inter1, ...}
            st1, st2 = st_pair[0], st_pair[1]
            if st2 == st:
                st1, st2 = st2, st1
            # st1, st2 = st, other_st
            if st1 in self.vertices:
                del self.vertices[st1]
            '''删掉self.vertices[st2]里对应line对应的inter'''
            for l_pr, inter in dic.items():
                #print(st_pair, l_pr, inter)
                l1, l2 = l_pr[0], l_pr[1]
                cond1 = l1 in self.vertices[st2]
                cond2 = l2 in self.vertices[st2]
                if cond1 or cond2:
                    if cond1: # if l1 in st2, then swap l1,l2
                        l2, l1 = l1, l2 
                    # l2 in st2
                    # print("in cond1 or cond2")
                    # print("del loop", st_pair, "\n")
                    # print(l_pr, inter)
                    val = self.vertices[st2][l2]
                    # print(st2, l2, val)
                    '''
                    删前需要检查是不是其他st的inter
                    '''
                    inter_of_other_st = False
                    st_pair_remain = [key for key in self.intersections if st not in key]
                    for cur_st_pr in st_pair_remain:
                        temp = self.intersections[cur_st_pr]
                        if inter in temp.values():
                            inter_of_other_st = True
                            break
                    if inter_of_other_st:
                        break
                    # 只有这一个交点，在dic里删掉这个key
                    if [inter] == val:
                        # print("# 只有这一个交点，在dic里删掉这个key\n")
                        # print([inter], val, "\n")
                        del self.vertices[st2][l2] # bug fix for deletion
                        # print("after: ", self.vertices)
                    # 多个交点，list里只删这一个交点，保留其他
                    elif inter in val:
                        # print("# 多个交点，list里只删这一个交点，保留其他\n")
                        # print(inter, val, "\n")
                        self.vertices[st2][l2].remove(inter)
                        # print("after: ", self.vertices)
            # vertices
            '''{
                st2: {
                    line1: [inter1, ...], 
                    line2: [inter1, ...],
                    ...}
                ,...
            }
            '''

            '''del intersections'''
            del self.intersections[st_pair] # fix bug on del

        # del st name
        del self.streets[st]
        #self.check_status()
            

    def add(self, st_name, coord_list, line_list):
        if self.street_exist(st_name):
            '''Err msg: street name exists'''
            raise Exception("Street name already exists!")
        '''加到self.st里面'''
        self.streets[st_name] = (coord_list, line_list)
        '''后续重新计算交点 pairwise cal'''
        for st in self.streets:
            if st == st_name:
                continue
            self.new_st_intersection(st, st_name)

    
    def mod(self, st_name, coord_list, line_list):
        if not self.street_exist(st_name):
            '''Err msg: street doesn't exists'''
            raise Exception("Cannot modify street that doesn't exist!")
        '''后续重新计算交点'''
        self.del_st_inter(st_name)
        self.streets[st_name] = (coord_list, line_list)
        for st in self.streets:
            if st == st_name:
                continue
            self.new_st_intersection(st, st_name)

    
    def rm(self, st_name):
        if not self.street_exist(st_name):
            '''Err msg: street doesn't exists'''
            raise Exception("Cannot remove street that doesn't exist!")
        #del self.streets[st_name]
        '''后续重新计算交点'''
        self.del_st_inter(st_name)
        

    def gg(self):
        '''
        1.get vertices
        2.get edges
        3.print in given format
        做一个set操作，去重
        '''
        vertices = []
        edges = [] # [(1,2),...]
        id_auto_incr = 1
        vertex_id = {}
        '''get vertices'''
        for st, dic in self.vertices.items():
            for line, inter_list in dic.items():
                cur_vertices = inter_list + [line.src, line.dst]
                cur_vertices.sort(key=lambda p: (p.x, p.y))
                for ver in cur_vertices:
                    if ver not in vertex_id:
                        vertex_id[ver] = id_auto_incr
                        vertices.append(ver)
                        id_auto_incr += 1
                for idx in range(len(cur_vertices)-1):
                    v1 = vertex_id[cur_vertices[idx]]
                    v2 = vertex_id[cur_vertices[idx+1]]
                    # fix bug: v1 = v2
                    if v1 == v2:
                        continue
                    to_append = (v1, v2)
                    if to_append not in edges:
                        edges.append(to_append)
        '''
        modification starts here
        '''
        print(f"V {len(vertices)}")
        # for index, coord in enumerate(vertices):
        #     print(f"  {index+1}:  {coord}")
        # print("}")
        print("E {", end="")
        for i, edge in enumerate(edges):
            v1, v2 = edge[0], edge[1]
            if i != len(edges)-1:
                print(f"<{v1},{v2}>,", end="")
            else:
                print(f"<{v1},{v2}>", end="")
        print("}", flush=True)
        #self.check_status()


# fix bug: checking st name
def check_st_name(st):
    # st name cannot be empty
    if st.strip() == "":
        return False
    for char in st:
        # modified for st names to include number
        #if not (char.isalpha() or char == " "):
        if not (char.isalnum() or char == " "):
            return False
    return True


'''
1.re分开
2.检查格式
3.output

Not passed cases:
    None for now

Passed:
1.FAQ: 
    Q1-Street Names: We do NOT allow street names with no characters whatsoever or only with white spaces.
        Street name must have at least one alphabet character (i.e., [a-zA-Z]).
    Q4-acceptable coordinate input 
    Q10-White-space
    Q14-Street Name Case Insensitive
    Q16-Street Name Alphabetical and space characters only
2.Other cases:
    a. redundency: rm "st" 12; gg 12
    b. lack spaces: add "weber" (1,2)(3,4) (5,6)
    c. street name: add '' (1,2); add '12' (1,2)
'''
# return command_type, street, coord_list, line_list
def parse_line(command):
    # fix bug: input: "  gg"
    command = command.strip()
    #print(command)
    # deals with empty input
    if command == '':
        return None, None, None, None
    if command.startswith("rm"):
        command += " "
    pattern = re.compile(r'''
        (add|mod|rm)\s+"([^"]+)"\s+
        |
        (gg)
    ''', re.VERBOSE | re.IGNORECASE)
    match = pattern.match(command)
    #print(match)
    if match:
        command_type = match.group(1)
        coord_part = command[match.end():]
        # deals with gg 
        if not command_type: # gg is captured in group 3
            command_type = match.group(3)
            if not re.fullmatch(r"\s*", coord_part):
                raise Exception("Redundent command after typing 'gg'!")
            return command_type, None, None, None
        # other commands: add mod rm
        '''command case: now sure when to convert'''
        command_type = command_type.lower() # convert to lower case
        street = match.group(2)
        street = street.lower() # fix bug: street names should be case insensitive
        '''Err if invalid street naming format'''
        if not check_st_name(street): # bug fix
            raise Exception("Street names cannot be empty and contain alphabetical and space characters only!")
        
        '''raise Err if no space bewtween pair of brackets'''
        if coord_part.find(")(") != -1:
            raise Exception("Need space between ) and ( !")
        coord_list = None
        line_list = None
        # check add and mod
        if command_type in ["add", "mod"]:
            # valid like: ( 1,  2  ), ( -1,  -2 )
            coord_pattern = re.compile(r'\(\s*(-?\d+)\s*,\s*(-?\d+)\s*\)')
            coordinates = coord_pattern.findall(coord_part)
            
            # add "oo street" (1， 1) (2， 2) (3， 3) (4， 4) 
            if coord_pattern.sub('', coord_part).strip():
                # print("if coord_pattern.sub('', coord_part).strip()")
                # print(coord_pattern.sub('', coord_part).strip())
                # print(coord_pattern)
                # print(coordinates)
                raise Exception("Invalid command format!", command)
                
            
            coord_list = []
            for coord in coordinates:
                '''string form'''
                x, y = coord[0], coord[1]
                pt = point(x, y)
                coord_list.append(pt)
            # fix bug: if only one coord
            if len(coord_list) < 2:
                raise Exception("Specified command has less than 2 coordinates entered")
            # multiple same input coord
            if len(set(coord_list)) < len(coord_list):
                raise Exception("Cotain one coordinate more than one time")
            line_list = [line(coord_list[i], coord_list[i+1]) 
                         for i in range(len(coord_list)-1)]
                
        # check rm
        elif command_type in ["rm"]:
            if not re.fullmatch(r"\s*", coord_part):
                raise Exception("Redundent command after typing 'rm'!")
                
        return command_type, street, coord_list, line_list

    else:
        raise Exception("Invalid command format!", command)
        

    

def main():
    # YOUR MAIN CODE GOES HERE
    street_map = graph()

    # sample code to read from stdin.
    # make sure to remove all spurious print statements as required
    # by the assignment
    while True:
        try:
            line = sys.stdin.readline()
            if line == "":
                break
            #print("read a line:", line)

            ''' 
            parse & check input
            throw exception when necessary
            '''
            command_type, street, coord_list, line_lsit = parse_line(line)
            '''-------------要删的---------------'''
            #print(command_type, street, coord_list, line_lsit)
            if command_type == "add":
                street_map.add(street, coord_list, line_lsit)
            elif command_type == "mod":
                street_map.mod(street, coord_list, line_lsit)
            elif command_type == "rm":
                street_map.rm(street)
            elif command_type == "gg":
                street_map.gg()
            '''for debug only---NEED TO DELETE!!!!=------'''
            # street_map.check_status()
        except Exception as e:
            print(f"Error: {e}", file=sys.stderr)

    # 要删的
    #print("Finished reading input")
    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == "__main__":
    main()
