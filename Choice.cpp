#include "Choice.h"

const sf::Color Choice::HIGHLIGHT_COLOR(0, 0, 139);
const sf::Color Choice::DEFAULT_TEXT_COLOR(0, 100, 0);

Choice::Choice()
{
    IsEnable = true;
    Description = "";
    Color = DEFAULT_TEXT_COLOR;

    Text.setFont(DEFAULT_FONT);
    Text.setString(Label);
    Text.setCharacterSize(DEFAULT_FONT_SIZE);
}

void Choice::SetLabel(std::string l)
{
    Label = l;
    Text.setString(Label);
    //DEFAULT_FONT.get
}

void Choice::DrawChoice(bool isMouseOver, sf::RenderWindow & window)
{
    Text.setColor(Color);
    if (isMouseOver)
    {
        if (IsEnable)
        {
            Text.setColor(HIGHLIGHT_COLOR);
        }
    }
    // Draw choice
    window.draw(Text);
}

void Choice::SetChoicePosMiddle(std::vector<Choice> & choices, size_t screenWidth, size_t screenHeight)
{
    size_t size = choices.size();
    if (size > 0)
    {
        float totalHeight;
        sf::FloatRect localBound = choices[0].Text.getLocalBounds();
        float fontHeight = localBound.height;
        float lineHeight = fontHeight * 1.5f;
        totalHeight = fontHeight;
        if (size == 1)
        {
            sf::Vector2f pos((float)((screenWidth - localBound.width) / 2), (float)((screenHeight - fontHeight) / 2));
            choices[0].Text.setPosition(pos);
        }
        else if (size > 1)
        {
            totalHeight = totalHeight + ((lineHeight + fontHeight)*size);
            float startPos = (screenHeight - totalHeight) / 2;
            float nextPos = startPos;
            for (size_t i = 0; i < size; i++)
            {
                localBound = choices[i].Text.getLocalBounds();
                sf::Vector2f pos((float)((screenWidth - localBound.width) / 2), nextPos);
                choices[i].Text.setPosition(pos);
                nextPos = nextPos + fontHeight + lineHeight;
            }
        }
    }
}
