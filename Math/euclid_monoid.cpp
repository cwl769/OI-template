struct Monoid {
  i64 x, y, sy;
  Monoid():x(),y(),sy(){}//unity is (0, 0, 0)
  Monoid(i64 _x, i64 _y, i64 _sy):x(_x),y(_y),sy(_sy){}
  Monoid operator * (const Monoid& b) const {
    Monoid ans;
    ans.x = x + b.x;
    ans.y = y + b.y;
    ans.sy = sy + b.sy + y * b.x;//$\sum_{S_1}y+\sum_{S_2}y+y_1\sum_{S_2}1$
    return ans;
  }
};
Monoid U(0, 1, 0);
Monoid R(1, 0, 0);