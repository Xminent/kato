#ifndef MOZART_COMPONENTS_GUILD_ICON_HPP
#define MOZART_COMPONENTS_GUILD_ICON_HPP

#include <mozart/components/widget.hpp>

namespace mozart
{
struct GuildIcon : Widget {
	Q_OBJECT

    public:
	explicit GuildIcon(const std::optional<QString> &name,
			   QWidget *parent = nullptr);

	[[nodiscard]] uint64_t id() const
	{
		return m_id;
	}

	void unclick();

    signals:
	void left_clicked(GuildIcon *guild_id);

    private:
	void setup_ui();
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;

	QString m_hash;
	bool m_clicked{};
	uint64_t m_id{};
	std::optional<QString> m_name;
};
} // namespace mozart

#endif // MOZART_COMPONENTS_GUILD_ICON_HPP
