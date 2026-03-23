#ifndef MONGOOSE_DIALOG_HPP
#define MONGOOSE_DIALOG_HPP

#include <mongoose/mongoose.hpp>


namespace mongoose
{
   class Dialog final
   {
   public:
      enum class Style : uint8_t
      {
         information, warning, critical
      };

      enum class OptionTag : uint8_t
      {
         none, primary, cancel
      };

   public:
      explicit Dialog( std::string_view title, std::string_view body ) noexcept;

      auto set_width( double const width ) noexcept -> void { width_ = width; }
      auto set_style( Style const style ) noexcept -> void { style_ = style; }
      // auto set_icon( DialogIcon const icon ) -> void;

      auto with_option( std::string_view option, OptionTag tag = OptionTag::none ) & noexcept -> Dialog&;
      auto with_option( std::string_view option, OptionTag tag = OptionTag::none ) && noexcept -> Dialog&&;
      auto clear_options( ) noexcept -> void;

      [[nodiscard]] auto last_option_selection( ) const noexcept -> char const*;

      auto display( ) noexcept -> char const*; // NOLINT(*-use-nodiscard)

   private:
      std::string_view const title_{};
      std::string_view const body_{};

      double width_{ 300. };
      Style style_{ Style::information };

   private:
      static constexpr size_t max_options_count_{ 6U };
      std::array<char const*, max_options_count_> options_{};
      uint8_t options_count_{ 0U };

      std::optional<uint8_t> default_option_{ std::nullopt };
      std::optional<uint8_t> cancel_option_{ std::nullopt };

      std::optional<uint8_t> last_selected_option_{ std::nullopt };

      auto append_option( std::string_view option, OptionTag tag ) noexcept -> void;
   };


   inline auto Dialog::with_option( std::string_view const option, OptionTag const tag ) & noexcept -> Dialog&
   {
      append_option( option, tag );
      return *this;
   }


   inline auto Dialog::with_option( std::string_view const option, OptionTag const tag ) && noexcept -> Dialog&&
   {
      append_option( option, tag );
      return std::move( *this );
   }


   inline auto Dialog::clear_options( ) noexcept -> void
   {
      options_count_        = 0U;
      default_option_       = std::nullopt;
      cancel_option_        = std::nullopt;
      last_selected_option_ = std::nullopt;
   }


   inline auto Dialog::last_option_selection( ) const noexcept -> char const*
   {
      if ( not last_selected_option_.has_value( ) )
      {
         return nullptr;
      }
      return options_[last_selected_option_.value( )];
   }


   inline auto Dialog::append_option( std::string_view const option, OptionTag const tag ) noexcept -> void
   {
      if ( options_count_ >= max_options_count_ || option.empty( ) )
      {
         return;
      }
      //
      options_[options_count_] = option.data( );
      //
      switch ( tag )
      {
         case OptionTag::none: break;
         //
         case OptionTag::primary:
         {
            default_option_ = options_count_;
            break;
         }
         //
         case OptionTag::cancel:
         {
            cancel_option_ = options_count_;
            break;
         }
      }
      //
      ++options_count_;
   }
}


#endif //!MONGOOSE_DIALOG_HPP
