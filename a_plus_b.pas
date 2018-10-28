program prog1540728829;
var
a:longint;
b:longint;
procedure print_c(val:char);
begin
    write(val)
end;

procedure print_s(val:string);
begin
    write(val)
end;

procedure print_i(val:longint);
begin
    write(val);
end;

procedure print_f(val:real);
begin
    write(val);
end;

function input_c():char;
var
ch : char;
begin
    read(ch);
    input_c := ch;
end;

function input_f():real;
var
fl : real;
begin
    read(fl);
    input_f := fl;
end;

function input_i():longint;
var
i : longint;
begin
    read(i);
    input_i := i;
end;

function input_s():string;
var
s : string;
begin
    read(s);
    input_s := s;
end;
function plus(a:longint;b:longint):longint;
begin
plus := a + b;
end;
begin
a := input_i;
b := input_i;
print_i(a + b);
end.
