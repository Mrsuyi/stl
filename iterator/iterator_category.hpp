#pragma once

namespace mrsuyi
{
struct input_iterator_tag
{
};
struct output_iterator_tag
{
};
struct forward_iterator_tag : public mrsuyi::input_iterator_tag
{
};
struct bidirectional_iterator_tag : public mrsuyi::forward_iterator_tag
{
};
struct random_access_iterator_tag : public mrsuyi::bidirectional_iterator_tag
{
};
}
