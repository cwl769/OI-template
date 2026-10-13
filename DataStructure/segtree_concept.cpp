template <typename Data>
concept Mergeable = requires(Data a, Data b)
  { { merge(a, b) } -> std::convertible_to<Data>;};
template <typename Data, typename Delta>
concept Addable = requires(Data a, Delta d) { a += d; };
template <typename Data, typename Action>
concept RangeChangeable =
  requires(Data& a, Action b) { a.act(b, 1ll, 1ll); };
template <typename Action>
concept Composable =
  requires(Action a, Action b) { a.compose(b); };
template <typename Data, typename Action>
concept SegtreeRC_concept = Mergeable<Data> &&
  RangeChangeable<Data, Action> && Composable<Action>;