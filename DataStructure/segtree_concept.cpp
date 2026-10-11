template <typename Data>
concept Mergeable = requires(Data a, Data b) {
  { merge(a, b) } -> std::convertible_to<Data>;
};
template <typename Data, typename Delta>
concept Addable = requires(Data a, Delta d) { a.add(d); };
